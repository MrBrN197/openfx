#ifndef _ofxsParam_H_
#define _ofxsParam_H_

/*
  OFX Support Library, a library that skins the OFX plug-in API with C++ classes.
  Copyright (C) 2004-2005 The Foundry Visionmongers Ltd
  Author Bruno Nicoletti bruno@thefoundry.co.uk

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name The Foundry Visionmongers Ltd, nor the names of its 
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The Foundry Visionmongers Ltd
1 Wardour St
London W1D 6PA
England



*/


/** @file This file contains core code that wraps OFX parameters with C++ classes.

This file only holds code that is visible to a plugin implementation, and so hides much
of the direct OFX objects and any library side only functions.

The classes that skin parameters are broken into two sets, those used during the description phase, 
eg OFX::IntParamDescriptor and those representing instances eg, OFX::IntParamInstance. The members on
each represent the actions that can be carried out on those particular OFX objects.

 */

#include "ofxsCore.h"

/** @brief Nasty macro used to define empty protected copy ctors and assign ops */
#define mDeclareProtectedAssignAndCC(CLASS) \
  CLASS &operator=(const CLASS &v1) {assert(false); return *this;}	\
  CLASS(const CLASS &v) {assert(false); } 

/** @brief The core 'OFX Support' namespace, used by plugin implementations. All code for these are defined in the common support libraries.
 */
namespace OFX {

    /* forward class declarations of the  descriptors */
    class ParamDescriptor;
    class ValueParamDescriptor;
    class IntParamDescriptor;
    class Int2DParamDescriptor;
    class Int3DParamDescriptor;    
    class DoubleParamDescriptor;
    class Double2DParamDescriptor;
    class Double3DParamDescriptor;    
    class StringParamDescriptor;
    class RGBAParamDescriptor;
    class RGBParamDescriptor;
    class BooleanParamDescriptor;
    class ChoiceParamDescriptor;
    class GroupParamDescriptor;
    class PageParamDescriptor;
    class PushButtonParamDescriptor;
    class CustomParamDescriptor;
    class ParamSetDescriptor;

    /* forward class declarations of the instances */
    class Param;
    class ValueParam;
    class IntParam;
    class Int2DParam;
    class Int3DParam;    
    class DoubleParam;
    class Double2DParam;
    class Double3DParam;    
    class RGBAParam;
    class RGBParam;
    class StringParam;
    class BooleanParam;
    class ChoiceParam;
    class CustomParam;
    class GroupParam;
    class PageParam;
    class PushButtonParam;
    class ParamSet;


    /** @brief Enumerates the different types of parameter */
    enum ParamTypeEnum {eDummyParam,
                        eStringParam,
                        eIntParam,
                        eInt2DParam,
                        eInt3DParam,
                        eDoubleParam,
                        eDouble2DParam,
                        eDouble3DParam,
                        eRGBParam,
                        eRGBAParam,
                        eBooleanParam,
                        eChoiceParam,
                        eCustomParam,
                        eGroupParam,
                        ePageParam,
                        ePushButtonParam};

    /** @brief Enumerates the different types of cache invalidation */
    enum CacheInvalidationEnum {eCacheInvalidateValueChange,
                                eCacheInvalidateValueChangeToEnd,
                                eCacheInvalidateValueAll};

    /** @brief Enumerates how we search for keys in an animating parameter */
    enum KeySearchEnum {eKeySearchBackwards,
                        eKeySearchNear,
                        eKeySearchForwards};

    /** @brief Enumerates the differing types of string params */
    enum StringTypeEnum {
        eStringTypeSingleLine,
        eStringTypeMultiLine,
        eStringTypeFilePath,
        eStringTypeDirectoryPath,
        eStringTypeLabel
    };

    /** @brief Enumerates the differing types of double params */
    enum DoubleTypeEnum {
        eDoubleTypePlain,
        eDoubleTypeAngle,
        eDoubleTypeScale,
        eDoubleTypeTime,
        eDoubleTypeAbsoluteTime,
        eDoubleTypeNormalisedX,
        eDoubleTypeNormalisedY,
        eDoubleTypeNormalisedXAbsolute,
        eDoubleTypeNormalisedYAbsolute,
        eDoubleTypeNormalisedXY,
        eDoubleTypeNormalisedXYAbsolute    
    };

    /** @brief turns a ParamTypeEnum into the char * that raw OFX uses */
    const char *
    mapParamTypeEnumToString(ParamTypeEnum v);

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Base class for all param descriptors */
    class ParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(ParamDescriptor);
        ParamDescriptor(void) {assert(false);}

    protected :
        std::string    _paramName;
        ParamTypeEnum  _paramType;
        PropertySet    _paramProps;

        /** @brief hidden constructors */
        ParamDescriptor(const std::string &name, ParamTypeEnum type, OfxPropertySetHandle props);

        friend class ParamSetDescriptor;
    public :
        /** @brief dtor */
        virtual ~ParamDescriptor();

        ParamTypeEnum getType(void) const {return _paramType;}

        /** @brief name */
        const std::string &getName(void) const {return _paramName;}

        /** @brief set the label properties in a plugin */
        void setLabels(const std::string &label, const std::string &shortLabel, const std::string &longLabel);

        /** @brief set the param hint */
        void setHint(const std::string &hint);

        /** @brief set the script name, default is the name it was created with */
        void setScriptName(const std::string &hint);

        /** @brief set the secretness of the param, defaults to false */
        void setIsSecret(bool v);

        /** @brief set the group param that is the parent of this one, default is to be ungrouped at the root level */
        void setParent(const GroupParamDescriptor &v);
    
        /** @brief whether the param is enabled, defaults to true */
        void setEnabled(bool v);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Used to implement dummy parameters for page positioning commands */
    class DummyParamDescriptor : public ParamDescriptor {
    public :
        /** @brief ctor */
        DummyParamDescriptor(const std::string &name) 
          : ParamDescriptor(name, eDummyParam, 0)
        {}  
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a value holding param */
    class ValueParamDescriptor : public ParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(ValueParamDescriptor);
        ValueParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        ValueParamDescriptor(const std::string &name, ParamTypeEnum type, OfxPropertySetHandle props);

        friend class ParamSetDescriptor;

    public :
        /** @brief dtor */
        ~ValueParamDescriptor();

        /** @brief set whether the param can animate, defaults to true in most cases */
        void setAnimates(bool v);

        /** @brief set whether the param is persistant, defaults to true */
        void setIsPersistant(bool v);

        /** @brief Set's whether the value of the param is significant (ie: affects the rendered image), defaults to true */
        void setEvaluateOnChange(bool v);
    
        /** @brief Set's how any cache should be invalidated if the parameter is changed, defaults to eCacheInvalidateValueChange */
        void setCacheInvalidation(CacheInvalidationEnum v);

        /// @brief Set whether the param should appear on any undo stack
        void setCanUndo(bool v);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a string param */
    class StringParamDescriptor : public ValueParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(StringParamDescriptor);
        StringParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        StringParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        friend class ParamSetDescriptor;

    public :
        /** @brief set the default value, default is 0 */
        void setDefault(const std::string &v);

        /** @brief sets the kind of the string param, defaults to eStringSingleLine */
        void setStringType(StringTypeEnum v);

        /** @brief if the string param is a file path, say that we are picking an existing file, rather than posibly specifying a new one, defaults to true */
        void setFilePathExists(bool v);    
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an integer param */
    class IntParamDescriptor : public ValueParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(IntParamDescriptor);
        IntParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        IntParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        friend class ParamSetDescriptor;

    public :
        /** @brief set the default value, default is 0 */
        void setDefault(int v);

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void setRange(int min, int max);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(int min, int max);
    };
    
    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an 2d integer param */
    class Int2DParamDescriptor : public ValueParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(Int2DParamDescriptor);
        Int2DParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        Int2DParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        friend class ParamSetDescriptor;

    public :
        /** @brief set the dimension labels */
        void setDimensionLabels(const std::string &x,
                                const std::string &y);

        /** @brief set the default value, default is 0 */
        void setDefault(int x, int y);

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void setRange(int minX, int minY,
                      int maxX, int maxY);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(int minX, int minY,
                            int maxX, int maxY);
    };
  
    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an 3d integer param */
    class Int3DParamDescriptor : public ValueParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(Int3DParamDescriptor);
        Int3DParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        Int3DParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        friend class ParamSetDescriptor;

    public :
        /** @brief set the dimension labels */
        void setDimensionLabels(const std::string &x,
                                const std::string &y,
                                const std::string &z);

        /** @brief set the default value, default is 0 */
        void setDefault(int x, int y, int z);

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void setRange(int minX, int minY, int minZ,
                      int maxX, int maxY, int maxZ);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(int minX, int minY, int minZ,
                            int maxX, int maxY, int maxZ);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Common base to all double param types */
    class BaseDoubleParamDescriptor : public ValueParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(BaseDoubleParamDescriptor);
        BaseDoubleParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        BaseDoubleParamDescriptor(const std::string &name, ParamTypeEnum type, OfxPropertySetHandle props);

        friend class ParamSetDescriptor;
    public :
        /** @brief set the type of the double param, defaults to eDoubleTypePlain */
        void setDoubleType(DoubleTypeEnum v);

        /** @brief set the sensitivity of any gui slider */
        void setIncrement(double v);

        /** @brief set the number of digits printed after a decimal point in any gui */
        void setDigits(int v);
    };
  
    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a double param */
    class DoubleParamDescriptor : public BaseDoubleParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(DoubleParamDescriptor);
        DoubleParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        DoubleParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        friend class ParamSetDescriptor;

    public :
        /** @brief if the double type is Absolute time, show a time marker on the time line if possible */
        void setShowTimeMarker(bool v);

        /** @brief set the default value, default is 0 */
        void setDefault(double v);

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void setRange(double min, double max);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(double min, double max);
    };
  
    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a 2D double param */
    class Double2DParamDescriptor : public BaseDoubleParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(Double2DParamDescriptor);
        Double2DParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        Double2DParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        friend class ParamSetDescriptor;

    public :
        /** @brief set the dimension labels */
        void setDimensionLabels(const std::string &x,
                                const std::string &y);

        /** @brief set the default value, default is 0 */
        void setDefault(double x, double y);

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void setRange(double minX, double minY,
                      double maxX, double maxY);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(double minX, double minY,
                            double maxX, double maxY);
    };
  
    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a 3D double param */
    class Double3DParamDescriptor : public BaseDoubleParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(Double3DParamDescriptor);
        Double3DParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        Double3DParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        friend class ParamSetDescriptor;

    public :
        /** @brief set the dimension labels */
        void setDimensionLabels(const std::string &x,
                                const std::string &y,
                                const std::string &z);

        /** @brief set the default value, default is 0 */
        void setDefault(double x, double y, double z);

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void setRange(double minX, double minY, double minZ,
                      double maxX, double maxY, double maxZ);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(double minX, double minY, double minZ,
                            double maxX, double maxY, double maxZ);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an RGB colour param */
    class RGBParamDescriptor : public ValueParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(RGBParamDescriptor);
        RGBParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        RGBParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        // so it can make one
        friend class ParamSetDescriptor;
    public :
        /** @brief set the default value */
        void setDefault(double r, double g, double b);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an RGBA colour param */
    class RGBAParamDescriptor : public ValueParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(RGBAParamDescriptor);
        RGBAParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        RGBAParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        // so it can make one
        friend class ParamSetDescriptor;
    public :
        /** @brief set the default value */
        void setDefault(double r, double g, double b, double a);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a boolean param */
    class BooleanParamDescriptor : public ValueParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(BooleanParamDescriptor);
        BooleanParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        BooleanParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        // so it can make one
        friend class ParamSetDescriptor;
    public :
        /** @brief set the default value */
        void setDefault(bool v);
    };
  
    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a choice param */
    class ChoiceParamDescriptor : public ValueParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(ChoiceParamDescriptor);
        ChoiceParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        ChoiceParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        // so it can make one
        friend class ParamSetDescriptor;
    public :
        /** @brief set the default value */
        void setDefault(int v);

        /** @brief append an option, default is to have not there */
        void appendOption(const std::string &v);
    
        /** @brief how many options do we have */
        int getNOptions(void);
    
        /** @brief clear all the options so as to add some new ones in */
        void resetOptions(void);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a group param, not much to it really */
    class GroupParamDescriptor : public ParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(GroupParamDescriptor);
        GroupParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        GroupParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        // so it can make one
        friend class ParamSetDescriptor;
    public :
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a page param, not much to it really */
    class PageParamDescriptor : public ParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(PageParamDescriptor);
        PageParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        PageParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        // so it can make one
        friend class ParamSetDescriptor;
    public :

        /** @brief adds a child parameter. Note the two existing pseudo params, gColumnSkip  and gRowSkip */
        void addChild(const ParamDescriptor &p);

        /** @brief dummy page positioning parameter to be passed to @ref OFX::PageParamDescriptor::addChild */
        static DummyParamDescriptor gSkipRow;

        /** @brief dummy page positioning parameter to be passed to @ref OFX::PageParamDescriptor::addChild */
        static DummyParamDescriptor gSkipColumn;
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a push button param, not much to it at all */
    class PushButtonParamDescriptor : public ParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(PushButtonParamDescriptor);
        PushButtonParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        PushButtonParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        // so it can make one
        friend class ParamSetDescriptor;
    public :
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a custom param, haven't added animation support yet */
    class CustomParamDescriptor : public ValueParamDescriptor {
    protected :
        mDeclareProtectedAssignAndCC(CustomParamDescriptor);
        CustomParamDescriptor(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        CustomParamDescriptor(const std::string &name, OfxPropertySetHandle props);

        // so it can make one
        friend class ParamSetDescriptor;
    public :
        /** @brief set the default value of the param */
        void setDefault(const std::string &v);    
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Describes a set of properties */
    class ParamSetDescriptor { 
    protected :
        mDeclareProtectedAssignAndCC(ParamSetDescriptor);

        /** @brief calls the raw OFX routine to define a param */
        void defineRawParam(const std::string &name, ParamTypeEnum paramType, OfxPropertySetHandle &props);

        /** @brief Define a param descriptor of the given type */
        template <class T> bool
        defineParamDescriptor(const std::string &name, ParamTypeEnum paramType, T * &paramPtr)
        {
            paramPtr = NULL;

            // have we made it already in this param set and is it of the correct type
            if(ParamDescriptor *param  = findPreviouslyDefinedParam(name)) {
                if(param->getType() == paramType) {
                    paramPtr = (T *) param; // could be a dynamic cast here
                    return true;
                }
                else
                    return false; // SHOULD THROW SOMETHING HERE!!!!!!!
            }
            else {
                // ok define one and add it in
                OfxPropertySetHandle props;
                defineRawParam(name, paramType, props);
    
                // make out support descriptor class
                paramPtr = new T(name, props);

                // add it to our map of described ones
                _definedParams[name] = paramPtr;
            }
            return true;
        }

    protected :
        /** @brief Properties that belong to this param set */
        PropertySet _paramSetProps;

        /** @brief Parameter set handle */
        OfxParamSetHandle _paramSetHandle;

        /** @brief Set of all previously defined parameters, defined on demand */
        std::map<std::string, ParamDescriptor *> _definedParams;

        /** @brief Hidden ctor */
        ParamSetDescriptor(void);

        /** @brief set the param set handle */
        void setParamSetHandle(OfxParamSetHandle h);

        /** @brief find a param in the map */
        ParamDescriptor *findPreviouslyDefinedParam(const std::string &name);

    public :
        virtual ~ParamSetDescriptor();
        /** @brief tries to fetch a ParamDescriptor, returns 0 if it isn't there*/
        ParamDescriptor* getParamDescriptor(const std::string& name) const;

        /** @brief estabilishes the order of page params. Do it by calling it in turn for each page */
        void setPageParamOrder(PageParamDescriptor &p);

        /** @brief Define an integer param */
        IntParamDescriptor *defineIntParam(const std::string &name);

        /** @brief Define a 2D integer param */
        Int2DParamDescriptor *defineInt2DParam(const std::string &name);

        /** @brief Define a 3D integer param */
        Int3DParamDescriptor *defineInt3DParam(const std::string &name);
    
        /** @brief Define an double param */
        DoubleParamDescriptor *defineDoubleParam(const std::string &name);

        /** @brief Define a 2D double param */
        Double2DParamDescriptor *defineDouble2DParam(const std::string &name);

        /** @brief Define a 3D double param */
        Double3DParamDescriptor *defineDouble3DParam(const std::string &name);
    
        /** @brief Define a string param */
        StringParamDescriptor *defineStringParam(const std::string &name);

        /** @brief Define a RGBA param */
        RGBAParamDescriptor *defineRGBAParam(const std::string &name);

        /** @brief Define an RGB  param */
        RGBParamDescriptor *defineRGBParam(const std::string &name);

        /** @brief Define a Boolean  param */
        BooleanParamDescriptor *defineBooleanParam(const std::string &name);

        /** @brief Define a Choice param */
        ChoiceParamDescriptor *defineChoiceParam(const std::string &name);

        /** @brief Define a group param */
        GroupParamDescriptor *defineGroupParam(const std::string &name);

        /** @brief Define a Page param */
        PageParamDescriptor *definePageParam(const std::string &name);

        /** @brief Define a push button param */
        PushButtonParamDescriptor *definePushButtonParam(const std::string &name);

        /** @brief Define a custom param */
        CustomParamDescriptor *defineCustomParam(const std::string &name);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Base class for all param instances */
    class Param {
    protected :
        // don't ever use these!
        Param &operator=(const Param &v1) {assert(false); return *this;} 
        Param(const Param &v) : _paramSet(v._paramSet) {assert(false); } 
        Param(void) {assert(false);}
    
    protected :
        std::string    _paramName;
        ParamTypeEnum  _paramType;
        PropertySet    _paramProps;
        OfxParamHandle _paramHandle;
        const ParamSet      *_paramSet; // who do I belong to

        /** @brief hidden constructor */
        Param(const ParamSet *paramSet, const std::string &name, ParamTypeEnum type, OfxParamHandle handle);

        friend class ParamSet;
    public :
        /** @brief dtor */
        virtual ~Param();

        /** @brief get name */
        const std::string &getName(void) const;
    
        /** @brief, set the label properties in a plugin */
        void setLabels(const std::string &label, const std::string &shortLabel, const std::string &longLabel);

        /** @brief return the derived type of this parameter */
        ParamTypeEnum getType(void) const {return _paramType;}

        /** @brief set the secretness of the param, defaults to false */
        void setIsSecret(bool v);

        /** @brief set the param hint */
        void setHint(const std::string &hint);

        /** @brief whether the param is enabled */
        void setEnabled(bool v);

        /** @brief fetch the labels */
        void getLabels(std::string &label, std::string &shortLabel, std::string &longLabel) const;
    
        /** @brief get whether the param is secret */
        bool getIsSecret(void) const;

        /** @brief whether the param is enabled */
        bool getIsEnable(void) const;
    
        /** @brief get the param hint */
        std::string getHint(void) const;

        /** @brief get the script name */
        std::string getScriptName(void) const;

        /** @brief get the group param that is the parent of this one */
        GroupParam *getParent(void) const;
    };
  
    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a value holding param */
    class ValueParam : public Param {
    protected :
        mDeclareProtectedAssignAndCC(ValueParam);
        ValueParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        ValueParam(const ParamSet *paramSet, const std::string &name, ParamTypeEnum type, OfxParamHandle handle);
      
        friend class ParamSet;
    public :
        /** @brief dtor */
        ~ValueParam();

        /** @brief Set's whether the value of the param is significant (ie: affects the rendered image) */
        void setEvaluateOnChange(bool v);
    
        /** @brief is the param animating */
        bool getIsAnimating(void) const;

        /** @brief is the param animating */
        bool getIsAutoKeying(void) const;
    
        /** @brief is the param animating */
        bool getIsPersistant(void) const;
    
        /** @brief Get's whether the value of the param is significant (ie: affects the rendered image) */
        bool getEvaluateOnChange(void) const;

        /** @brief Get's whether the value of the param is significant (ie: affects the rendered image) */
        CacheInvalidationEnum getCacheInvalidation(void) const;

        /** @brief if the param is animating, the number of keys in it, otherwise 0 */
        unsigned int getNumKeys(void);

        /** @brief get the time of the nth key, nth must be between 0 and getNumKeys-1 */
        double getKeyTime(int nthKey) throw(OFX::Exception::Suite, std::out_of_range);

        /** @brief find the index of a key by a time */
        int getKeyIndex(double time, 
                        KeySearchEnum searchDir);
    
        /** @brief deletes a key at the given time */
        void deleteKeyAtTime(double time);

        /** @brief delete all the keys */
        void deleteAllKeys(void);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an integer param */
    class IntParam : public ValueParam {
    protected :
        mDeclareProtectedAssignAndCC(IntParam);
        IntParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        IntParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        friend class ParamSet;
    public :
        /** @brief set the default value */
        void setDefault(int v);

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void setRange(int min, int max);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(int min, int max);

        /** @brief het the default value */
        void getDefault(int &v);

        /** @brief het the default value */
        int getDefault(void) {int v; getDefault(v); return v;}

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void getRange(int &min, int &max);

        /** @brief set the display min and max, default is to be the same as the range param */
        void getDislayRange(int &min, int &max);

        /** @brief get value */
        void  getValue(int &v);

        /** @brief and a nicer one */
        int getValue(void) {int v; getValue(v); return v;}

        /** @brief get the value at a time */
        void getValueAtTime(double t, int &v);

        /** @brief and a nicer one */
        int getValueAtTime(double t) {int v; getValueAtTime(t, v); return v;}

        /** @brief set value */
        void setValue(int v);

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, int v);
    };  

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an integer param */
    class Int2DParam : public ValueParam {
    protected :
        mDeclareProtectedAssignAndCC(Int2DParam);
        Int2DParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        Int2DParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        friend class ParamSet;
    public :
        /** @brief set the default value, default is 0 */
        void setDefault(int x, int y);

        /** @brief set the default value, default is 0 */
        void setDefault(const OfxPointI &v) {setDefault(v.x, v.y);}

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void setRange(int minX, int minY,
                      int maxX, int maxY);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(int minX, int minY,
                            int maxX, int maxY);

        /** @brief het the default value */
        void getDefault(int &x, int &y);

        /** @brief get the default value */
        OfxPointI getDefault(void) {OfxPointI v; getDefault(v.x, v.y); return v;}

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void getRange(int &minX, int &minY,
                      int& maxX, int &maxY);

        /** @brief set the display min and max, default is to be the same as the range param */
        void getDislayRange(int &minX, int &minY,
                            int &maxX, int &maxY);

        /** @brief get value */
        void getValue(int &x, int &y);

        /** @brief get the  value */
        OfxPointI getValue(void) {OfxPointI v; getValue(v.x, v.y); return v;}

        /** @brief get the value at a time */
        void getValueAtTime(double t, int &x, int &y);

        /** @brief get the  value */
        OfxPointI getValueAtTime(double t) {OfxPointI v; getValueAtTime(t, v.x, v.y); return v;}

        /** @brief set value */
        void setValue(int x, int y);

        /** @brief set the current value */
        void setValue(const OfxPointI &v) {setValue(v.x, v.y);}

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, int x, int y);

        /** @brief set the current value */
        void setValueAtTime(double t, const OfxPointI &v) {setValueAtTime(t, v.x, v.y);}
    };  

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an integer param */
    class Int3DParam : public ValueParam {
    protected :
        mDeclareProtectedAssignAndCC(Int3DParam);
        Int3DParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        Int3DParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        friend class ParamSet;
    public :
        /** @brief set the default value, default is 0 */
        void setDefault(int x, int y, int z);

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void setRange(int minX, int minY, int minZ,
                      int maxX, int maxY, int maxZ);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(int minX, int minY, int minZ,
                            int maxX, int maxY, int maxZ);

        /** @brief het the default value */
        void getDefault(int &x, int &y, int &z);

        /** @brief set the hard min/max range, default is INT_MIN, INT_MAX */
        void getRange(int &minX, int &minY, int &minZ,
                      int& maxX, int &maxY, int &maxZ);

        /** @brief set the display min and max, default is to be the same as the range param */
        void getDislayRange(int &minX, int &minY, int &minZ,
                            int& maxX, int &maxY, int &maxZ);

        /** @brief get value */
        void getValue(int &x, int &y, int &z);

        /** @brief get the value at a time */
        void getValueAtTime(double t, int &x, int &y, int &z);

        /** @brief set value */
        void setValue(int x, int y, int z);

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, int x, int y, int z);
    };  

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Common base to all double param types */
    class BaseDoubleParam : public ValueParam {
    protected :
        mDeclareProtectedAssignAndCC(BaseDoubleParam);
        BaseDoubleParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        BaseDoubleParam(const ParamSet *paramSet, const std::string &name, ParamTypeEnum type, OfxParamHandle handle);

        friend class ParamSet;
    public :
        /** @brief set the sensitivity of any gui slider */
        void setIncrement(double v);

        /** @brief set the number of digits printed after a decimal point in any gui */
        void setDigits(int v);

        /** @brief get the sensitivity of any gui slider */
        void getIncrement(double &v);

        /** @brief get the number of digits printed after a decimal point in any gui */
        void getDigits(int &v);

        /** @brief get the type of the double param */
        void getDoubleType(DoubleTypeEnum &v);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an doubleeger param */
    class DoubleParam : public BaseDoubleParam {
    protected :
        mDeclareProtectedAssignAndCC(DoubleParam);
        DoubleParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        DoubleParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        friend class ParamSet;
    public :
        /** @brief set the default value */
        void setDefault(double v);

        /** @brief if the double type is Absolute time, show a time marker on the time line if possible */
        void setShowTimeMarker(bool v);

        /** @brief set the hard min/max range, default is DOUBLE_MIN, DOUBLE_MAX */
        void setRange(double min, double max);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(double min, double max);

        /** @brief het the default value */
        void getDefault(double &v);

        /** @brief het the default value */
        double getDefault(void) {double v; getDefault(v); return v;}

        /** @brief set the hard min/max range, default is DOUBLE_MIN, DOUBLE_MAX */
        void getRange(double &min, double &max);

        /** @brief set the display min and max, default is to be the same as the range param */
        void getDislayRange(double &min, double &max);

        /** @brief get value */
        void getValue(double &v);

        /** @brief get value */
        double getValue(void) {double v; getValue(v); return v;}

        /** @brief get the value at a time */
        void getValueAtTime(double t, double &v);

        /** @brief get value */
        double getValueAtTime(double t) {double v; getValueAtTime(t, v); return v;}

        /** @brief set value */
        void setValue(double v);

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, double v);

        /** @brief differentiate the param */
        void differentiate(double t, double &v);

        /** @brief differentiate the param  */
        double differentiate(double t) {double v; differentiate(t, v); return v;}

        /** @brief integrate the param */
        void integrate(double t1, double t2, double &v);

        /** @brief integrate the param */
        double integrate(double t1, double t2) {double v; integrate(t1, t2, v); return v;}
    };  

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an doubleeger param */
    class Double2DParam : public BaseDoubleParam {
    protected :
        mDeclareProtectedAssignAndCC(Double2DParam);
        Double2DParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        Double2DParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        friend class ParamSet;
    public :
        /** @brief set the default value, default is 0 */
        void setDefault(double x, double y);

        /** @brief set the hard min/max range, default is DOUBLE_MIN, DOUBLE_MAX */
        void setRange(double minX, double minY,
                      double maxX, double maxY);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(double minX, double minY,
                            double maxX, double maxY);

        /** @brief het the default value */
        void getDefault(double &x, double &y);

        /** @brief set the hard min/max range, default is DOUBLE_MIN, DOUBLE_MAX */
        void getRange(double &minX, double &minY,
                      double& maxX, double &maxY);

        /** @brief set the display min and max, default is to be the same as the range param */
        void getDislayRange(double &minX, double &minY,
                            double &maxX, double &maxY);

        /** @brief get value */
        void getValue(double &x, double &y);

        /** @brief get the value at a time */
        void getValueAtTime(double t, double &x, double &y);

        /** @brief set value */
        void setValue(double x, double y);

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, double x, double y);

        /** @brief differentiate the param */
        void differentiate(double t, double &x, double &y);

        /** @brief differentiate the param  */
        OfxPointD differentiate(double t) {OfxPointD v; differentiate(t, v.x, v.y); return v;}

        /** @brief integrate the param */
        void integrate(double t1, double t2, double &x, double &y);

        /** @brief integrate the param */
        OfxPointD integrate(double t1, double t2) {OfxPointD v; integrate(t1, t2, v.x, v.y); return v;}
    };  

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an doubleeger param */
    class Double3DParam : public BaseDoubleParam {
    protected :
        mDeclareProtectedAssignAndCC(Double3DParam);
        Double3DParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        Double3DParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        friend class ParamSet;
    public :
        /** @brief set the default value, default is 0 */
        void setDefault(double x, double y, double z);

        /** @brief set the hard min/max range, default is DOUBLE_MIN, DOUBLE_MAX */
        void setRange(double minX, double minY, double minZ,
                      double maxX, double maxY, double maxZ);

        /** @brief set the display min and max, default is to be the same as the range param */
        void setDislayRange(double minX, double minY, double minZ,
                            double maxX, double maxY, double maxZ);

        /** @brief het the default value */
        void getDefault(double &x, double &y, double &z);

        /** @brief set the hard min/max range, default is DOUBLE_MIN, DOUBLE_MAX */
        void getRange(double &minX, double &minY, double &minZ,
                      double& maxX, double &maxY, double &maxZ);

        /** @brief set the display min and max, default is to be the same as the range param */
        void getDislayRange(double &minX, double &minY, double &minZ,
                            double& maxX, double &maxY, double &maxZ);

        /** @brief get value */
        void getValue(double &x, double &y, double &z);

        /** @brief get the value at a time */
        void getValueAtTime(double t, double &x, double &y, double &z);

        /** @brief set value */
        void setValue(double x, double y, double z);

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, double x, double y, double z);

        /** @brief differentiate the param */
        void differentiate(double t, double &x, double &y, double &z);

        /** @brief differentiate the param  */
        Ofx3DPointD differentiate(double t) {Ofx3DPointD v; differentiate(t, v.x, v.y, v.z); return v;}

        /** @brief integrate the param */
        void integrate(double t1, double t2, double &x, double &y, double &z);

        /** @brief integrate the param */
        Ofx3DPointD integrate(double t1, double t2) {Ofx3DPointD v; integrate(t1, t2, v.x, v.y, v.z); return v;}
    };  
  
    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an RGB param */
    class RGBParam : public ValueParam {
    protected :
        mDeclareProtectedAssignAndCC(RGBParam);
        RGBParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        RGBParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        friend class ParamSet;
    public :
        /** @brief set the default value, default is 0 */
        void setDefault(double r, double g, double b);

        /** @brief get default value */
        void getDefault(double &r, double &g, double &b);

        /** @brief get value */
        void getValue(double &r, double &g, double &b);

        /** @brief get the value at a time */
        void getValueAtTime(double t, double &r, double &g, double &b);

        /** @brief set value */
        void setValue(double r, double g, double b);

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, double r, double g, double b);
    };  
 
  
    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up an RGB param */
    class RGBAParam : public ValueParam {
    protected :
        mDeclareProtectedAssignAndCC(RGBAParam);
        RGBAParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        RGBAParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        friend class ParamSet;
    public :
        /** @brief set the default value, default is 0 */
        void setDefault(double r, double g, double b, double a);

        /** @brief get default value */
        void getDefault(double &r, double &g, double &b, double &a);

        /** @brief get value */
        void getValue(double &r, double &g, double &b, double &a);

        /** @brief get the value at a time */
        void getValueAtTime(double t, double &r, double &g, double &b, double &a);

        /** @brief set value */
        void setValue(double r, double g, double b, double a);

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, double r, double g, double b, double a);
    };  

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a string param */
    class StringParam : public ValueParam {
    protected :
        mDeclareProtectedAssignAndCC(StringParam);
        StringParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        StringParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        friend class ParamSet;

    public :
        /** @brief set the default value */
        void setDefault(const std::string &v);

        /** @brief get the default value */
        void getDefault(std::string &v);

        /** @brief get value */
        void getValue(std::string &v);

        /** @brief get the value at a time */
        void getValueAtTime(double t, std::string &v);

        /** @brief set value */
        void setValue(const std::string &v);

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, const std::string &v);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a choice param */
    class ChoiceParam : public ValueParam {
    protected :
        mDeclareProtectedAssignAndCC(ChoiceParam);
        ChoiceParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        ChoiceParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        // so it can make one
        friend class ParamSet;
    public :
        /** @brief set the default value */
        void setDefault(int v);

        /** @brief get the default value */
        void getDefault(int &v);

        /** @brief how many options do we have */
        int getNOptions(void);

        /** @brief append an option, default is to have not there */
        void appendOption(const std::string &v);
    
        /** @brief clear all the options so as to add some new ones in */
        void resetOptions(void);

        /** @brief get value */
        void getValue(int &v);

        /** @brief get the value at a time */
        void getValueAtTime(double t, int &v);

        /** @brief set value */
        void setValue(int v);

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, int v);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a boolean param */
    class BooleanParam : public ValueParam {
    protected :
        mDeclareProtectedAssignAndCC(BooleanParam);
        BooleanParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        BooleanParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        // so it can make one
        friend class ParamSet;
    public :
        /** @brief set the default value */
        void setDefault(bool v);

        /** @brief get the default value */
        void getDefault(bool &v);

        /** @brief get the default value */
        bool getDefault(void) {bool v; getDefault(v); return v;}

        /** @brief get value */
        void getValue(bool &v);

        /** @brief get value */
        bool getValue(void) {bool v; getValue(v); return v;}

        /** @brief get the value at a time */
        void getValueAtTime(double t, bool &v);

        /** @brief get value */
        bool getValueAtTime(double t) {bool v; getValueAtTime(t, v); return v;}

        /** @brief set value */
        void setValue(bool v);

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, bool v);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a group param */
    class GroupParam : public Param {
    protected :
        mDeclareProtectedAssignAndCC(GroupParam);
        GroupParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        GroupParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        // so it can make one
        friend class ParamSet;
    public :
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a group param */
    class PageParam : public Param {
    protected :
        mDeclareProtectedAssignAndCC(PageParam);
        PageParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        PageParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        // so it can make one
        friend class ParamSet;
    public :
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a custom param, not animation support yet */
    class CustomParam : public ValueParam {
    protected :
        mDeclareProtectedAssignAndCC(CustomParam);
        CustomParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        CustomParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        // so it can make one
        friend class ParamSet;
    public :
        /** @brief set the default value of the param */
        void setDefault(const std::string &v);    

        /** @brief get the default value of the param */
        void getDefault(std::string &v);    

        /** @brief get value */
        void getValue(std::string &v);

        /** @brief get the value at a time */
        void getValueAtTime(double t, std::string &v);

        /** @brief set value */
        void setValue(const std::string &v);

        /** @brief set the value at a time, implicitly adds a keyframe */
        void setValueAtTime(double t, const std::string &v);
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief Wraps up a push button param, not much to it at all */
    class PushButtonParam : public Param {
    protected :
        mDeclareProtectedAssignAndCC(PushButtonParam);
        PushButtonParam(void) {assert(false);}

    protected :
        /** @brief hidden constructor */
        PushButtonParam(const ParamSet *paramSet, const std::string &name, OfxParamHandle handle);

        // so it can make one
        friend class ParamSet;
    public :
    };

    ////////////////////////////////////////////////////////////////////////////////
    /** @brief A set of parameters in a plugin instance */
    class ParamSet { 
    protected :
        mDeclareProtectedAssignAndCC(ParamSet);
        ParamTypeEnum getParamType(const std::string& name) const;
    private :
        /** @brief Properties that belong to this param set */
        PropertySet _paramSetProps;

        /** @brief Parameter set handle */
        OfxParamSetHandle _paramSetHandle;

        /** @brief Set of all previously fetched parameters, created on demand */
        mutable std::map<std::string, Param *> _fetchedParams;

        /** @brief see if we have a param of the given name in out map */
        Param *findPreviouslyFetchedParam(const std::string &name) const;

        /** @brief calls the raw OFX routine to define a param */
        void fetchRawParam(const std::string &name, ParamTypeEnum paramType, OfxParamHandle &handle) const;


        /** @brief Fetch a param of the given name and type */
        template <class T> void
        fetchParam(const std::string &name, ParamTypeEnum paramType, T * &paramPtr) const
        {
            paramPtr = NULL;

            // have we made it already in this param set and is it an int?
            if(Param *param  = findPreviouslyFetchedParam(name)) {
                if(param->getType() == paramType) {
                    paramPtr = (T *) param; // could be a dynamic cast here
                }
                else
                  throw OFX::Exception::TypeRequest("Fetching param and attempting to return the wrong type");
            }
            else {
                // ok define one and add it in
                OfxParamHandle paramHandle;
                fetchRawParam(name, paramType, paramHandle);
    
                // make out support descriptor class
                paramPtr = new T(this, name, paramHandle);

                // add it to our map of described ones
                _fetchedParams[name] = paramPtr;
            }
        }

    protected :
        /** @brief Hidden ctor */
        ParamSet(void);

        /** @brief set the param set handle */
        void setParamSetHandle(OfxParamSetHandle h);

    public :
        virtual ~ParamSet();

        bool paramExists(const std::string& name) const;

        /// open an undoblock
        void beginEditBlock(const std::string &name);

        /// close an undoblock
        void endEditBlock();

        Param* getParam(const std::string& name) const;

        /** @brief Fetch an integer param */
        IntParam *fetchIntParam(const std::string &name) const;

        /** @brief Fetch a 2D integer param */
        Int2DParam *fetchInt2DParam(const std::string &name) const;

        /** @brief Fetch a 3D integer param */
        Int3DParam *fetchInt3DParam(const std::string &name) const;
    
        /** @brief Fetch an double param */
        DoubleParam *fetchDoubleParam(const std::string &name) const;

        /** @brief Fetch a 2D double param */
        Double2DParam *fetchDouble2DParam(const std::string &name) const;

        /** @brief Fetch a 3D double param */
        Double3DParam *fetchDouble3DParam(const std::string &name) const;
    
        /** @brief Fetch a string param */
        StringParam *fetchStringParam(const std::string &name) const;

        /** @brief Fetch a RGBA param */
        RGBAParam *fetchRGBAParam(const std::string &name) const;

        /** @brief Fetch an RGB  param */
        RGBParam *fetchRGBParam(const std::string &name) const;

        /** @brief Fetch a Boolean  param */
        BooleanParam *fetchBooleanParam(const std::string &name) const;

        /** @brief Fetch a Choice param */
        ChoiceParam *fetchChoiceParam(const std::string &name) const;

        /** @brief Fetch a group param */
        GroupParam *fetchGroupParam(const std::string &name) const;

        /** @brief Fetch a page param */
        PageParam *fetchPageParam(const std::string &name) const;

        /** @brief Fetch a push button param */
        PushButtonParam *fetchPushButtonParam(const std::string &name) const;

        /** @brief Fetch a custom param */
        CustomParam *fetchCustomParam(const std::string &name) const;
    };

};

// undeclare the protected assign and CC macro
#undef mDeclareProtectedAssignAndCC

#endif