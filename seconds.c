// Isaiah Cabugos seconds.c

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <asm/param.h>
#include <linux/jiffies.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

#define BUFFER_SIZE 128

#define PROC_NAME "seconds"
#define MESSAGE "Seconds\n"


int jiffStart = 0;

/**
 * Function prototypes
 */
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
};


/* This function is called when the module is loaded. */
int proc_init(void)
{

        // creates the /proc/seconds entry
        // the following function call is a wrapper for
        // proc_create_data() passing NULL as the last argument
        proc_create(PROC_NAME, 0, NULL, &proc_ops);

        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

        jiffStart = jiffies;
      	printk(KERN_INFO "Value of Jiffies in Proc_Init: %lu\nValue of HZ: %d\n", jiffStart, HZ);

	return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {

        // removes the /proc/hello entry
        remove_proc_entry(PROC_NAME, NULL);

        // printk(KERN_INFO "GCD of 3300 and 24: %lu\n", gcd(3300, 24));
	    // printk(KERN_INFO "Value of Jiffies in Proc_Exit: %lu\n", jiffies);
        printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/hello is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;

        if (completed) {
                completed = 0;
                return 0;
        }

        completed = 1;

        int diff = jiffies - jiffStart;
        rv = sprintf(buffer, "Reported Number of Seconds since process start: %d\n", diff / HZ);

        // copies the contents of buffer to userspace usr_buf
        copy_to_user(usr_buf, buffer, rv);

        return rv;
}

/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");


/*========================================================
======================== SCRIPT ===========================

sudo -p "osc"
sudo dmesg -c 
clear 

printf "beginning script...\n\n"

printf "now testing proc seconds\n"
sudo insmod seconds.ko

sleep 2
cat /proc/seconds
sleep 1
cat /proc/seconds

sudo rmmod seconds
sudo dmesg 
printf "finished testing proc seconds\n"

printf "\n...end of script\n\n"

====================== END SCRIPT =========================
==========================================================*/

/*========================================================
======================== OUTPUT ===========================

beginning script...

now testing proc seconds
Reported Number of Seconds since process start: 2
Reported Number of Seconds since process start: 3
[ 1379.081533] /proc/seconds created
[ 1379.081535] Value of Jiffies in Proc_Init: 4295237002
               Value of HZ: 250
[ 1382.143221] /proc/seconds removed
finished testing proc seconds

...end of script

====================== END OUTPUT =========================
==========================================================*/
