//
// Created by BraxtonN on 2/12/2018.
//

#ifndef SHARP_CLASSOBJECT_H
#define SHARP_CLASSOBJECT_H

#include "../../../stdimports.h"
#include "string.h"
#include "Field.h"
#include "symbol.h"

struct Method;

class ClassObject : public Symbol {
public:
    ClassObject()
            :
            Symbol(),
            fields(NULL),
            methods(NULL),
            super(NULL),
            interfaces(NULL),
            totalFieldCount(0),
            instanceFields(0),
            staticFields(0),
            methodCount(0),
            interfaceCount(0)
    {
        type = CLASS;
    }

    void init() { ClassObject(); }

    void operator=(const ClassObject* klass) {
        name = klass->name;
        fullName = klass->fullName;
        fields = klass->fields;
        methods = klass->methods;
        owner = klass->owner;
        guid = klass->guid;
        super = klass->super;
        totalFieldCount = klass->totalFieldCount;
        instanceFields = klass->instanceFields;
        staticFields = klass->staticFields;
        methodCount = klass->methodCount;
        type = klass->type;
    }

    Field *fields;
    Method **methods;
    ClassObject *super;
    ClassObject **interfaces;
    uInt staticFields;
    uInt instanceFields;
    uInt totalFieldCount;
    uInt methodCount;
    uInt interfaceCount;

    void free();

    Field* getfield(string name);
    uInt fieldindex(string name);

    bool isClassRelated(ClassObject *klass);
};

#endif //SHARP_CLASSOBJECT_H
