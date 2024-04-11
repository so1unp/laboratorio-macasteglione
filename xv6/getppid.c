/*
 * Invoca la llamada al sistema getppid().
 */
#include "types.h"
#include "user.h"
#include "date.h"

int main(void)
{
    // Descomentar una vez implementada la llamada al sistema.
    printf(1, "%d\n", getppid());
    exit();
}
