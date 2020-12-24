#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/stat.h>
#include <linux/kobject.h>

MODULE_AUTHOR("zjb");
MODULE_LICENSE("GPL");

struct kset *kset_p;
struct kset kset_c;
static char buf[20];

/* 函数声明 */
void obj_test_release(struct kobject *);
ssize_t kobj_test_show(struct kobject *,struct attribute *,char *);
ssize_t kobj_test_store(struct kobject *,struct attribute *,const char *,size_t);

static struct attribute test_attr =
{
		.name = "kobj_config",
		.mode = S_IRWXUGO,
};

static struct attribute *def_attrs[] =
{
		&test_attr,
		NULL,
};

static struct sysfs_ops obj_test_sysops =
{
		.show = kobj_test_show,
		.store = kobj_test_store,
};

static struct kobj_type ktype =
{
		.release = obj_test_release,
		.sysfs_ops = &obj_test_sysops,
		.default_attrs = def_attrs,
};

void obj_test_release(struct kobject *kobject)
{
		printk("[kobj_test: release!]\n");
}

ssize_t kobj_test_show(struct kobject *kobject,struct attribute *attr,char *buf)
{
		printk("Have show -->\n");
		printk("attrname: %s.\n",attr->name);
		sprintf(buf,"%s\n",attr->name);
		return strlen(attr->name) + 2;
}

ssize_t kobj_test_store(struct kobject *kobject,struct attribute *attr, const char *buf,size_t size)
{
		printk("Have store -->\n");
		printk("write: %s.\n",buf);
		return size;
}

static int kset_filter(struct kset *kset,struct kobject *kobj)
{
	printk("Filter: kobj %s.\n",kobj->name);
	return 1;
}

static const char *kset_name(struct kset *kset,struct kobject *kobj)
{
   //static char buf[20];
	printk("Name kobj %s.\n",kobj->name);
	sprintf(buf,"%s","kset_name");
	return buf;
}

static int kset_uevent(struct kset *kset,struct kobject *kobj, struct kobj_uevent_env *env)

{
	int i = 0;
	printk("uevent: kobj %s.\n",kobj->name);

	while(i < env->envp_idx)
	{
		printk("%s.\n",env->envp[i]);
		i ++;
	}

	return 0;
}

static struct kset_uevent_ops uevent_ops =
{
	.filter = kset_filter,
	.name = kset_name,
	.uevent = kset_uevent,
};


static int __init kset_test_init(void)
{
	int ret = 0;

	printk("kset test init!\n");

	/* 创建并注册 kset_p */
	kset_p = kset_create_and_add("kset_p",&uevent_ops,NULL);

	/* 添加 kset_c 到 kset_p */
	kobject_set_name(&kset_c.kobj,"kset_c");
	kset_c.kobj.kset = kset_p;

	/* 对于较新版本的内核，在注册 kset 之前，需要  
		 * 填充 kset.kobj 的 ktype 成员，否则注册不会成功 */
	kset_c.kobj.ktype = &ktype;
	ret = kset_register(&kset_c);

	if(ret)
		kset_unregister(kset_p);

	return 0;
}

static void __exit kset_test_exit(void)
{
	printk("kset test exit!\n");
	kset_unregister(kset_p);
	kset_unregister(&kset_c);

}

module_init(kset_test_init);
module_exit(kset_test_exit);
