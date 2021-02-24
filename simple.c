/**
 * simple.c
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <asm/param.h>
#include <linux/jiffies.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

static LIST_HEAD(birthday_list);

void append_birthday(int day, int month, int year){
	struct birthday* person = kmalloc(sizeof(*person), GFP_KERNEL);
	person-> day	= day;
	person-> month	= month;
	person-> year	= year;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);
}

void print_birthday_list(void) {
	struct birthday* ptr;
	list_for_each_entry(ptr, &birthday_list, list) {
		printk(KERN_INFO "%02d/%02d/%04d", ptr->day, ptr->month, ptr->year);
	}
	printk(KERN_INFO "\n");
}

void delete_birthday_list(void) {
	struct birthday *ptr, *next;
	list_for_each_entry_safe(ptr, next, &birthday_list, list) {
	list_del(&ptr->list);
	kfree(ptr);
	}
	printk(KERN_INFO "Birthday list deleted successfully\n");
}


/* This function is called when the module is loaded. */
int simple_init(void)
{
    	 printk(KERN_INFO "Loading Module\n");
     	 printk(KERN_INFO "%lu\n", GOLDEN_RATIO_PRIME);
      	 printk(KERN_INFO "Value of Jiffies: %lu\nValue of HZ: %d\n", jiffies, HZ);
      	// printk(KERN_INFO "Creating birthday list\n");
		// append_birthday(2, 6, 2000);
		// append_birthday(8, 6, 1999);
		// append_birthday(22, 5, 1492);
		// append_birthday(31, 3, 500);
		// append_birthday(15, 8, 80);
		// printk(KERN_INFO "Printing birthday list (dd/mm/yyyy)\n");
		// print_birthday_list();
	return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "%lu\n", gcd(3300, 24));
	printk(KERN_INFO "Value of Jiffies: %lu\n", jiffies);
	// printk(KERN_INFO "Deleting birthday list\n");
	// delete_birthday_list();
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

