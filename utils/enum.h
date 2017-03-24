#ifndef ENUM_H
#define ENUM_H
#define Fun 800
#define Pro 801
#define L   802
typedef enum { typeCon = 0, typeId, typeOpr } nodeEnum;
typedef enum { typeVar = 10, typeFun, typePro, typeArr, typeVoid, typeNone } symEnum;
typedef enum { integer = 20, boolean, arrInt, arrBool} typeEnum;
#endif 
