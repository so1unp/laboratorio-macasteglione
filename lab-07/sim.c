#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIRTUAL_MEMORY_SIZE 65536    // 64 KiB
#define PHYSICAL_MEMORY_SIZE 32768   // 32 KiB
#define PAGE_SIZE 4096               // 4 KiB
#define SECONDARY_STORAGE_SIZE 65536 // 64 KiB
#define PROCESS_TABLE_SIZE 256

#define NUM_PAGES (VIRTUAL_MEMORY_SIZE / PAGE_SIZE)
#define NUM_FRAMES (PHYSICAL_MEMORY_SIZE / PAGE_SIZE)
#define NUM_SWAP_PAGES (SECONDARY_STORAGE_SIZE / PAGE_SIZE)

struct Frame
{
    int pid;
    int page;
};

struct Process
{
    int *pages;
    int num_pages;
    int *isSwap;
};

typedef struct Frame frame_t;
typedef struct Process process_t;

frame_t physical_memory[NUM_FRAMES];
frame_t secondary_storage[NUM_SWAP_PAGES];

process_t *process_table[PROCESS_TABLE_SIZE];
int num_processes = 0;

int clock_hand = 0;
int time_counter = 0;
int *lru_times;
int i, j;

void iniciar_memoria()
{
    for (i = 0; i < NUM_FRAMES; i++)
    {
        physical_memory[i].pid = -1;
        physical_memory[i].page = -1;
    }

    for (i = 0; i < NUM_SWAP_PAGES; i++)
    {
        secondary_storage[i].pid = -1;
        secondary_storage[i].page = -1;
    }

    lru_times = (int *)malloc(NUM_FRAMES * sizeof(int));
    memset(lru_times, 0, NUM_FRAMES * sizeof(int));
}

process_t *get_or_create_process(int pid)
{
    if (process_table[pid] == NULL)
    {
        process_table[pid] = (process_t *)malloc(sizeof(process_t));
        process_table[pid]->pages = (int *)malloc(NUM_PAGES * sizeof(int));
        process_table[pid]->isSwap = (int *)malloc(NUM_PAGES * sizeof(int));

        for (i = 0; i < NUM_PAGES; i++)
        {
            process_table[pid]->pages[i] = -1;
            process_table[pid]->isSwap[i] = 0;
        }

        process_table[pid]->num_pages = 0;
        num_processes++;
    }
    return process_table[pid];
}

int find_free_frame()
{
    for (i = 0; i < NUM_FRAMES; i++)
        if (physical_memory[i].pid == -1)
            return i;

    return -1;
}

int find_free_swap()
{
    for (i = 0; i < NUM_SWAP_PAGES; i++)
        if (secondary_storage[i].pid == -1)
            return i;

    return -1;
}

void swap_out(int frame_index)
{
    int swap_index = find_free_swap();

    if (swap_index == -1)
    {
        fprintf(stderr, "Swap space full! Cannot swap out.\n");
        exit(EXIT_FAILURE);
    }

    int pid = physical_memory[frame_index].pid;
    int page = physical_memory[frame_index].page;

    secondary_storage[swap_index] = physical_memory[frame_index];
    process_table[pid]->isSwap[page] = 1;
    physical_memory[frame_index].pid = -1;
    physical_memory[frame_index].page = -1;
}

void swap_in(int pid, int page, int frame_index)
{
    int swap_index = -1;

    for (i = 0; i < NUM_SWAP_PAGES; i++)
        if (secondary_storage[i].pid == pid && secondary_storage[i].page == page)
        {
            swap_index = i;
            break;
        }

    if (swap_index == -1)
    {
        physical_memory[frame_index].pid = pid;
        physical_memory[frame_index].page = page;
        return;
    }

    physical_memory[frame_index] = secondary_storage[swap_index];
    process_table[pid]->isSwap[page] = 0;
    secondary_storage[swap_index].pid = -1;
    secondary_storage[swap_index].page = -1;
}

void access_page(int pid, int page, char algorithm)
{
    process_t *proc = get_or_create_process(pid);
    int frame_index = -1;

    for (i = 0; i < NUM_FRAMES; i++)
        if (physical_memory[i].pid == pid && physical_memory[i].page == page)
        {
            frame_index = i;
            break;
        }

    if (frame_index == -1)
    {
        frame_index = find_free_frame();

        if (frame_index == -1)
        {
            if (algorithm == 'f')
            {
                frame_index = clock_hand;
                clock_hand = (clock_hand + 1) % NUM_FRAMES;
            }
            else if (algorithm == 'l')
            {
                int lru_index = 0;

                for (i = 1; i < NUM_FRAMES; i++)
                    if (lru_times[i] < lru_times[lru_index])
                        lru_index = i;

                frame_index = lru_index;
            }
            swap_out(frame_index);
        }

        swap_in(pid, page, frame_index);
        physical_memory[frame_index].pid = pid;
        physical_memory[frame_index].page = page;
    }

    lru_times[frame_index] = time_counter++;
    proc->pages[page] = frame_index;
}

void print_memory_state()
{
    printf("Tabla de páginas por proceso:\n");

    for (i = 0; i < PROCESS_TABLE_SIZE; i++)
        if (process_table[i] != NULL)
        {
            printf("Proceso %d: ", i);

            for (j = 1; j < NUM_PAGES; j++)
                if (process_table[i]->pages[j] == -1)
                    printf("- ");
                else
                    (process_table[i]->isSwap[j] == 1) ? printf("%ds ", process_table[i]->pages[j] + 1) : printf("%dr ", process_table[i]->pages[j] + 1);

            printf("\n");
        }

    printf("Memoria física:\n");

    for (i = 0; i < NUM_FRAMES; i++)
        if (physical_memory[i].pid == -1)
            printf("- ");
        else
            printf("%d.%d ", physical_memory[i].pid, physical_memory[i].page);

    printf("\n");
    printf("Almacenamiento secundario:\n");

    for (i = 0; i < NUM_SWAP_PAGES; i++)
        if (secondary_storage[i].pid == -1)
            printf("- ");
        else
            printf("%d.%d ", secondary_storage[i].pid, secondary_storage[i].page);

    printf("\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s -f|-l\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char algorithm = argv[1][1];

    if (algorithm != 'f' && algorithm != 'l')
    {
        fprintf(stderr, "Invalid algorithm. Use -f for FIFO or -l for LRU.\n");
        exit(EXIT_FAILURE);
    }

    iniciar_memoria();
    int pid, page;

    while (scanf("%d %d", &pid, &page) != EOF)
        access_page(pid, page, algorithm);

    print_memory_state();
}