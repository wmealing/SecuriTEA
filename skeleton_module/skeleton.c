#define pr_fmt(fmt)  	KBUILD_MODNAME "->%s:%d: " fmt, __func__,__LINE__

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/string.h> 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wade Mealing");
MODULE_DESCRIPTION ("Skeleton kernel module");


/*
 * This function is called at module load time, (insmod or modprobe)
 * and creates the directory in procfs and the exploitable file..
 */
static int __init skeleton_init(void)
{
	pr_info("Hello world\n");
 
  return 0;
}

/*
 * This function cleans up the file /proc/exploitable when removing the
 * kernel module (rmmod)
 */

static void skeleton_cleanup(void) {
	pr_info("Exited early");
}

/* register entry/exit points for module */
module_init(skeleton_init);
module_exit(skeleton_cleanup);
