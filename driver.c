#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include "driver.h"

/*-----------------------------------------------------------------------------*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andre Luis Martinotto");
MODULE_DESCRIPTION("Disciplina SO");

/*----------------------------------------------------------------------------*/
#define DEVICE 60
#define DEVICE_NAME "so"
#define BUF_MSG 100

/*----------------------------------------------------------------------------*/

int init_device(void);
void cleanup_device(void);
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user *buffer, size_t length,loff_t * offset);
static ssize_t device_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset);
long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long
ioctl_param);
/*----------------------------------------------------------------------------*/

module_init(init_device);
module_exit(cleanup_device)

/*----------------------------------------------------------------------------*/
static int aberto = 0;
static char mensagem[BUF_MSG] = "01212M-p-p-p-pp-p-p-p--p-p-p-p----------------b-b-b-b--b-b-b-bb-b-b-b-0";
static char *ptr;

/*----------------------------------------------------------------------------*/
struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
	.unlocked_ioctl = device_ioctl,	
};

/*----------------------------------------------------------------------------*/
int init_device(){

	int ret;

	ret = register_chrdev(DEVICE, DEVICE_NAME, &fops);

	if (ret < 0) {
		printk("Erro ao carregar o dispositivo %d\n.",DEVICE);	
		return ret;
	}

	printk("O dispositivo %d foi carregado.\n", DEVICE);

	return 0;
}

/*----------------------------------------------------------------------------*/

void cleanup_device(){

	unregister_chrdev(DEVICE, DEVICE_NAME);
	printk("O dispositivo %d foi descarregado.\n", DEVICE);
}


/*----------------------------------------------------------------------------*/
static int device_open(struct inode *inode, struct file *file){
	

	if (aberto){
		return -EBUSY;
	}
	aberto++;
	ptr = mensagem;

	try_module_get(THIS_MODULE);
	return 0;
}

/*----------------------------------------------------------------------------*/
static int device_release(struct inode *inode, struct file *file){
	
	aberto--;

	module_put(THIS_MODULE);
	return 0;
}

/*----------------------------------------------------------------------------*/
static ssize_t device_read(struct file *file, char __user * buffer, size_t length, loff_t * offset){

	int i, bytes_read = 0;

	if ( *ptr == 0){
		return 0;

	}

	for (i = 0; *ptr;  i++){
		put_user(*(ptr++), buffer++);
		bytes_read++;
	}

	printk("Leu %d bytes correspondendo a mensagem: %s", bytes_read, mensagem);

	return bytes_read;
}

/*----------------------------------------------------------------------------*/
static ssize_t device_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset){
	
	int i;

	for (i = 0; i < length; i++){
		get_user(mensagem[i], buffer + i);
	}
	ptr = mensagem;


	printk("Escreveu a mensagem: %s\n",mensagem);

	return i;
}

/*----------------------------------------------------------------------------*/

long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param){
	int length;
	char *temp = NULL;
	switch (ioctl_num) {
	case IOCTL_SET_SPEAKER:
		temp = (char *)ioctl_param;
		length = 0;
		while(*temp ){
			length++;
			temp++;
		}
		device_write(file, (char *)ioctl_param, length, 0);
		break;
		default:
			printk("Essa operacao nao e permitida.\n");
			return -1;
	}
	return 0;
}
