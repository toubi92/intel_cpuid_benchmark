//#include <stdint.h>

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

int64_t benchmark_cpuid_nothing(void) 
{				
	int64_t start, end;
	unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;	
	unsigned long flags;
	static int cpu_num = -1, cur_num;
	
	preempt_disable(); /*we disable preemption on our CPU*/
	cur_num = smp_processor_id();
	if(cpu_num != cur_num)
	{
		cpu_num = cur_num;
		pr_info("CPUID check : Running on CPU %d\n", cpu_num);
	}
	local_irq_save(flags); /*we disable hard interrupts on our CPU*/
	local_irq_disable();
	
	asm volatile ("CPUID\n\t"
	 "RDTSC\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
	"%rax", "%rbx", "%rcx", "%rdx");
	
	//Function to measure: Nothing
	
	asm volatile("RDTSCP\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t"
	 "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
	"%rax", "%rbx", "%rcx", "%rdx");
	
	local_irq_restore(flags); /*we enable hard interrupts on our CPU*/
	preempt_enable();/*we enable preemption*/
	
	start = ( ((int64_t)cycles_high << 32) | cycles_low );
	end = ( ((int64_t)cycles_high1 << 32) | cycles_low1 );
	
	return end - start;
}

static long bm_mod_ioctl(struct file *file, unsigned num, uintptr_t param)
{
	int64_t cycles;
	(void)file;

	cycles = benchmark_cpuid_nothing();
	copy_to_user((void *)param, &cycles, sizeof cycles);
	
	return 0;
}

static int bm_mod_open(struct inode *inode, struct file *file)
{
	(void)inode;
	(void)file;
	return 0;
}

static int bm_mod_release(struct inode *inode, struct file *file)
{
	(void)inode;	
	(void)file;
	return 0;
}

static struct file_operations fops = {
	.open = bm_mod_open,
	.release = bm_mod_release,
	.unlocked_ioctl = bm_mod_ioctl,
};

static struct miscdevice bm_mod_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "bm_mod!io",
	.fops = &fops,
	.mode = 0666,
};

int bm_mod_init(void)
{
	int ret;
	
	ret = misc_register(&bm_mod_dev);
	if (ret != 0) {
		return -1;
	}
	
	return 0;
}

void bm_mod_exit(void)
{
	misc_deregister(&bm_mod_dev);
}

module_init(bm_mod_init)
module_exit(bm_mod_exit)
MODULE_LICENSE("GPL");