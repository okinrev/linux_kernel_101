/*#ifndef KPLAYGROUND_H
#define KPLAYGROUND_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// Prototypes des fonctions
static int __init module_start(void);
static void __exit module_end(void);

#endif */

#ifndef KPLAYGROUND_H
#define KPLAYGROUND_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>          // Pour l'enregistrement des fichiers de périphériques
#include <linux/device.h>       // Pour la création du périphérique
#include <linux/cdev.h>         // Pour la gestion des périphériques caractères
#include <linux/uaccess.h>      // Pour copier les données vers et depuis l'espace utilisateur

// Prototypes des fonctions
static int kplayground_init(void);
static void kplayground_exit(void);
static int __init module_start(void);
static void __exit module_end(void);
static int kplayground_open(struct inode*, struct file*);
static int kplayground_release(struct inode*, struct file*);
static ssize_t kplayground_read(struct file*, char*, size_t, loff_t*);
static ssize_t kplayground_write(struct file*, const char*, size_t, loff_t*);

#endif
