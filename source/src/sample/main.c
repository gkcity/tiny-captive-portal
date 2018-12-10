#include <tiny_socket.h>
#include <tiny_snprintf.h>
#include <CaptivePortal.h>

static void runCaptivePortal(void)
{
    uint32_t ip = 0;
    uint8_t *a = (uint8_t *) &ip;

    CaptivePortal * cp = CaptivePortal_New(NULL, NULL);
    if (cp == NULL)
    {
        printf("CaptivePortal_New failed\n");
        return;
    }

    a[0] = 10;
    a[1] = 0;
    a[2] = 1;
    a[3] = 29;

    CaptivePortal_Run(cp, ip);
}

int main(void)
{
    tiny_socket_initialize();

    runCaptivePortal();

    tiny_socket_finalize();

    return 0;
}
