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
MODULE_DESCRIPTION ("Kernel module exploit example, stack canary investigation");

/* This module intends to show the stack canary mechanism */


#define BUFFERSIZE 1

static ssize_t leak_canary(struct file *file, char __user *buffer, size_t len, loff_t *offset);
static ssize_t exploitable_write(struct file *file, const char __user *buf, size_t len, loff_t *off); 

struct proc_dir_entry *proc_file_entry;

static const struct file_operations proc_file_fops = {
 .owner = THIS_MODULE,
 .write = exploitable_write,
 .read  = leak_canary,
};


/* We're going to leak the stack canary in here so that it can be used
 * So that we dont have to disable stack protection. 
 */


static ssize_t leak_canary(struct file *file, char __user *buffer, size_t len, loff_t *offset) {

	int retlen = 0;
	int canary = current->stack_canary;


	printk( KERN_INFO "canary  0x%08X", canary);  
	
	return retlen; 
}


static ssize_t exploitable_write(struct file *file, const char __user *buf, size_t len, loff_t *off) { 

  int modify_me = 0; 
  char buffer[BUFFERSIZE];

  memset(buffer, '\0', (size_t)BUFFERSIZE);

  printk(KERN_INFO "buffer at:    0x%p\n", buffer);
  printk(KERN_INFO "modify me at: 0x%p\n", &modify_me);

  /* Copy from userspace to kernelspace */
  if (__copy_from_user(&buffer, buf,len)) {
    return -EFAULT;
  }

  /* The objective of this exercise is to print this !*/

  if (modify_me == 0 ) { 
    printk(KERN_INFO "[.] modify_me is %d ! - overflow unsuccessful\n", modify_me);
  }
  else { 
    printk(KERN_INFO "[+] modify_me is not 0 ! - overflow successful\n");
    commit_creds( prepare_kernel_cred (0)); 
  }

  return len;
}

/*
 * This function is called at module load time, (insmod or modprobe)
 * and creates the directory in procfs and the exploitable file..
 */
static int __init canary_init(void)
{

  printk(KERN_INFO "exploit example - this should not be loaded on a production system\n");
  proc_file_entry = proc_create_data("exploitable", 0666, NULL, &proc_file_fops,NULL);
  return 0;
}

/*
 * This function cleans up the file /proc/exploitable when removing the
 * kernel module (rmmod)
 */

static void canary_cleanup(void) {
  remove_proc_entry("exploitable", NULL);  
  printk( KERN_INFO "Cleaning up canary-demo-module\n");
}

/* register entry/exit points for module */
module_init(canary_init);
module_exit(canary_cleanup);
