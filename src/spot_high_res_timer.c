#include <linux/module.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>

#define TIMER_DELAY_SECONDS     1
#define TIMER_DELAY_NANOSECONDS     0

static struct hrtimer spot_timer;
u64 start_time;
ktime_t timer_delay;

unsigned int timer_iterations;

/* Function to set the timer delay */
static void set_timer_delay(void)
{
        timer_delay = ktime_set(TIMER_DELAY_SECONDS, TIMER_DELAY_NANOSECONDS * 1000000L);
}

/* callback function for timer expiry */
static enum hrtimer_restart test_hrtimer_handler(struct hrtimer *timer)
{
        u64 current_time = jiffies;
        pr_info("timer got triggered after %umsec\n", jiffies_to_msecs(current_time - start_time));

        /* restart the timer by forwarding the current time delay */
        ktime_t now = hrtimer_cb_get_time(timer);                               
        hrtimer_forward(timer, now, timer_delay);

        start_time = current_time;
        timer_iterations++;

        return HRTIMER_RESTART;
}

static int __init spot_high_res_timer_init(void)
{
        /* hrtimer_init() is not available in 6.17 kernel,
        instead use hrtimer_setup() */
        hrtimer_setup(
                &spot_timer,
                &test_hrtimer_handler,
                CLOCK_MONOTONIC,
                HRTIMER_MODE_REL
        );
        
        set_timer_delay();
        timer_iterations = 0;

        start_time = jiffies;
        
        hrtimer_start(&spot_timer, timer_delay, HRTIMER_MODE_REL);        
        pr_info("Succeffully started the high resolution timer!\n");

        return 0;
}
static void __exit spot_high_res_timer_exit(void)
{
        hrtimer_cancel(&spot_timer);
        pr_info("Unregistered high resolution timer. Number of timer iterations: %d. Tschuss!\n",
                        timer_iterations);
}

/* Module load and unload functions */
module_init(spot_high_res_timer_init);
module_exit(spot_high_res_timer_exit);



/* Module Information */ 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rafnas AK / rafnasgit");
MODULE_DESCRIPTION("Module creates a high resolution timer at 1 seond granularity");
MODULE_VERSION("v0.1");