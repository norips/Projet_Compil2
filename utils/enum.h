#ifndef ENUM_H
#define ENUM_H
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define Fun 800
#define Pro 801
#define L   802
#define Acc 803 //Access to an array
#define Aft 804 //Affection to an array
typedef enum { typeCon = 0, typeId, typeOpr } nodeEnum;
typedef enum { typeVar = 10, typeFun, typePro, typeArr, typeVoid, typeNone } symEnum;
typedef enum { integer = 20, boolean, arrInt, arrBool} typeEnum;
#endif 
