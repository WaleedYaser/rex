#include "rex-core/log.h"
#include "stdio.h"

int main()
{
	rex_log_trace("%s", "rex");
	rex_log_debug("%s", "rex");
	rex_log_info("%s", "rex");
	rex_log_warn("%s", "rex");
	rex_log_error("%s", "rex");
	rex_log_fatal("%s", "rex");
	printf("hello, world!\n");
}
