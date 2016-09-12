/**
*******************************************************************************
* @file lkm_thread.c
* @author Keidan
* @par Project lkm_thread
* @copyright Copyright 2016 Keidan, all right reserved.
* @par License:
* This software is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY.
*
* Licence summary : 
*    You can modify and redistribute the sources code and binaries.
*    You can send me the bug-fix
*
* Term of the licence in in the file licence.txt.
*
* .____     ____  __.  _____    ___________.__                              .___
* |    |   |    |/ _| /     \   \__    ___/|  |_________   ____ _____     __| _/
* |    |   |      <  /  \ /  \    |    |   |  |  \_  __ \_/ __ \\__  \   / __ |
* |    |___|    |  \/    Y    \   |    |   |   Y  \  | \/\  ___/ / __ \_/ /_/ | 
* |_______ \____|__ \____|__  /   |____|   |___|  /__|    \___  >____  /\____ /
*         \/       \/       \/                  \/            \/     \/      \/ 
*
*******************************************************************************
*/
#include <linux/init.h>           /* Macros used to mark up functions e.g. __init __exit */
#include <linux/module.h>         /* Core header for loading LKMs into the kernel */
#include <linux/kernel.h>         /* Contains types, macros, functions for the kernel */
#include <linux/kthread.h>        /* Using kthreads for the flashing functionality */
#include <linux/delay.h>          /* Using this header for the msleep() function */

static struct task_struct *task; /* The pointer to the thread task */
static int the_value = 0; /* Simple variable used in the thread callback */

/**
 * @brief The thread callback.
 * @param arg The argument passed during the call of the kthread_run function.
 * @return return code
 */
static int lkm_thread_callback(void *arg){
  while(!kthread_should_stop()){            /* Returns true when kthread_stop() or equivalent is called */
    set_current_state(TASK_RUNNING);       /* prevent inadvertent sleeps temporarily (just an example) */
    the_value++; /* update the variable value */
    printk(KERN_INFO "[LKM_THREAD] new value from thread: %d\n", the_value);
    set_current_state(TASK_INTERRUPTIBLE); /* going to sleep but can be awoken if required */
    msleep(1000);                          /* retry in 1 second. */
  }
  return 0;
}

/**
 * @brief The LKM initialization function -create and run a kernel thread.
 * @return returns 0 if successful
 */
static int __init lkm_thread_init(void) {
   task = kthread_run(
     lkm_thread_callback, /* Callback function */
     NULL, /* Data pointer to be passed to the thread callback */
     "LKM_thread" /* Printf-style name for the thread. */
   ); 
   return 0;
}

/**
 * @brief The LKM cleanup function.
 */
static void __exit lkm_thread_exit(void) {  
  kthread_stop(task); /* Stop the thread task */
}

/****************************************************
 *    _____             .___    .__           .__       .__  __   
 *   /     \   ____   __| _/_ __|  |   ____   |__| ____ |__|/  |_ 
 *  /  \ /  \ /  _ \ / __ |  |  \  | _/ __ \  |  |/    \|  \   __\
 * /    Y    (  <_> ) /_/ |  |  /  |_\  ___/  |  |   |  \  ||  |  
 * \____|__  /\____/\____ |____/|____/\___  > |__|___|  /__||__|  
 *         \/            \/               \/          \/    
 *****************************************************/
/** 
 * @brief A module must use the module_init() module_exit() macros from linux/init.h, which 
 * identify the initialization function at insertion time and the cleanup function (as listed above)
 */
module_init(lkm_thread_init);
module_exit(lkm_thread_exit);

/* module infos */
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
MODULE_AUTHOR("Keidan (Kevin Billonneau)");
MODULE_DESCRIPTION("Simple LKM thread howto.");
