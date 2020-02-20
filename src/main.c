#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdint.h>
#include <sys/time.h>

#include <glib.h>
#include <Elementary.h>

#define GETTID()	((long)syscall(SYS_gettid))

static char *font_color[6] = {
	"\033[0m", /* reset */
	"\033[31m", /* red */
	"\033[32m", /* green */
	"\033[34m", /* blue */
	"\033[35m", /* magenta */
	"\033[36m" /* cyan */
};

#define _color_reset	"\033[0m"
#define _color_red		"\033[31m"
#define _color_green	"\033[32m"
#define _color_blue		"\033[35m"
#define _color_cyan		"\033[36m"


#define log(fmt, arg...)	fprintf(stderr, _color_green"%s(%d)> "_color_reset fmt"\n", __FUNCTION__, (int)__LINE__, ##arg)

int main(int argc, char *argv[])
{
	char *pkg = "LaySim Editor";

	elm_init(argc, argv);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	Evas_Object *win = elm_win_util_standard_add(pkg, pkg);
	elm_win_autodel_set(win, EINA_TRUE);
	evas_object_resize(win, 500, 500);
	evas_object_show(win);
	log("hello");

	elm_run();	
	elm_exit();

	return 0;
}
