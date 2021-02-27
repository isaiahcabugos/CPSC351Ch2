/**
 * hello.c
 *
 * Kernel module that communicates with /proc file system.
 * 
 * The makefile must be modified to compile this program.
 * Change the line "simple.o" to "hello.o"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

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

#define PROC_NAME "hello"
#define MESSAGE "Hello World\n"

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

        // creates the /proc/hello entry
        // the following function call is a wrapper for
        // proc_create_data() passing NULL as the last argument
        proc_create(PROC_NAME, 0, NULL, &proc_ops);

        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
     	printk(KERN_INFO "Golden Ratio: %lu\n", GOLDEN_RATIO_PRIME);
      	printk(KERN_INFO "Value of Jiffies in Proc_Init: %lu\nValue of HZ: %d\n", jiffies, HZ);

	return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {

        // removes the /proc/hello entry
        remove_proc_entry(PROC_NAME, NULL);

        printk(KERN_INFO "GCD of 3300 and 24: %lu\n", gcd(3300, 24));
	printk(KERN_INFO "Value of Jiffies in Proc_Exit: %lu\n", jiffies);
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

        rv = sprintf(buffer, "Hello World\n");

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

printf "now testing proc hello\n"
sudo insmod hello.ko

sleep 2
cat /proc/hello
sleep 1

sudo rmmod hello
sudo dmesg 
printf "finished testing proc hello\n"

printf "\n...end of script\n\n"

====================== END SCRIPT =========================
==========================================================*/

/*========================================================
======================== OUTPUT ===========================

beginning script...

now testing proc hello
Hello World
[  445.010966] /proc/hello created
[  445.010968] Golden Ratio: 11400862456688148481
[  445.010970] Value of Jiffies in Proc_Init: 4295003479
               Value of HZ: 250
[  448.083097] GCD of 3300 and 24: 12
[  448.083099] Value of Jiffies in Proc_Exit: 4295004248
[  448.083100] /proc/hello removed
finished testing proc hello

...end of script

====================== END OUTPUT =========================
==========================================================*/