#ifndef ENUM_H
#define ENUM_H
#define Fun 800
#define L   801
typedef enum { typeCon = 0, typeId, typeOpr } nodeEnum;
typedef enum { typeVar = 10, typeFun, typePro, typeArr, typeNone } symEnum;
typedef enum { integer = 20, boolean, arrInt, arrBool} typeEnum;
#endif 
