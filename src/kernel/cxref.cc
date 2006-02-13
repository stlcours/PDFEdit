#include "cxref.h"

CXref::~CXref()
{
       // TODO figure out what to deallocate 
       
       /* FIXME: uncoment when cache is ready.
       if(cache)
               delete cache;
        */
}

Object * CXref::changeObject(Ref ref, Object * instance)
{
       // discards from cache
       /* FIXME uncoment if cache is available
       if(cache)
               cache->discard(ref);
       */

       // searches in changedStorage
       // this is returned so it can be used for some
       // history information - value keeper is responsible for
       // deallocation
       Object * changed = changedStorage.get(ref);
       
       // insert new version to changedStorage
       // this is deep copy of given value
       Object * object = instance->clone();
       changedStorage.put(ref, object);

       // object has been newly created, so we will set value in
       // the newStorage to true (so we know, that the value has
       // been set after initialization)
       if(newStorage.contains(ref))
               newStorage.put(ref, true);
       
       // returns old version
       return changed;
}


void CXref::releaseObject(Ref ref)
{
       // gets previous counter of reference
       // NOTE: if reference is not on releaseObject, count is 0
       // and so its ok
       int count=releasedStorage.get(ref);
       
       // puts reference and increased counter association
       releasedStorage.put(ref, ++count);
}


Object * CXref::changeTrailer(char * name, Object * value)
{
      Dict * trailer = trailerDict.getDict(); 

      Object * prev = trailer->update(name, value);

      return prev;
}

Object * CXref::createObject(ObjType type, Ref * ref)
{
       int i=1;
       int num=-1, gen;
       
       // it's not possible to create indirect reference value or
       // any other internaly (by xpdf) used object types
       if(type==objRef || type==objError || type==objEOF || type==objNone)
               return 0;
       
       // goes through entries array in XRef class (xref entries)
       // and reuses first free entry with increased gen number.
       for(; i<size; i++)
               if(entries[i].type==xrefEntryFree)
               {
                       num=i;
                       gen=entries[i].gen+1;
                       break;
               }

       if(num==-1)
       {
               // no entry is free, so we have to add new number
               // TODO integer limitations
               num=i+1;
               gen=0;
       }

       Object * obj=(Object *)gmalloc(sizeof(Object));
       
       // initialize according type
       switch(type)
       {
               case objBool:
                       obj->initBool(false);
                       break;
               case objInt:
                       obj->initInt(0);
                       break;
               case objReal:
                       obj->initReal(0);
                       break;
               case objString:
                       obj->initString(NULL);
                       break;
               case objName:
                       obj->initName(NULL);
                       break;
               case objArray:
                       obj->initArray(this);
                       break;
               case objDict:
                       obj->initDict(this);
                       break;
               case objStream:
                       obj->initStream(NULL);
                       break;
               case objCmd:
                       obj->initCmd(NULL);
                       break;
               case objNull:
                       obj->initNull();
                       break;
               default:
                       // TODO assert

               break;
       }
       
       // registers newly created object to the newStorage
       // flag is set to false now and this is changed only if
       // initialized value is overwritten by change method
       // all objects with false flag should be ignored when
       // writing to a file
       Ref objRef={num, gen};
       newStorage.put(objRef, false);

       // sets reference parameter if not null
       if(ref)
               *ref=objRef;

       return obj;
}

bool CXref::knowsRef(Ref ref)
{
       // checks newly created object only with true flag
       // not found returns 0, so it's ok
       if(newStorage.get(ref))
               return true;

       // has to be found in entries
       if(entries[ref.num].type==xrefEntryFree)
               return false;
       // object number is ok, so also gen must fit
       return entries[ref.num].gen==ref.gen;
}

bool CXref::typeSafe(Object * obj1, Object * obj2)
{
       if(!obj1 || !obj2)
               return false;

       Object dObj1=*obj1, dObj2=*obj2;    // object for dereferenced 
                                           // values - we assume, it's
                                           // not indirect
       // types for direct values.
       ObjType type1=obj1->getType(), type2=obj2->getType();              
       
       // checks indirect
       if(type1==objRef)
       {
               fetch(dObj1.getRef().num, dObj1.getRef().gen, &dObj1);
               type1=dObj1.getType();
               // has to free object
               dObj1.free();
       }
       if(type2==objRef)
       {
               fetch(dObj2.getRef().num, dObj2.getRef().gen, &dObj2);
               type2=dObj2.getType();
               // has to free object
               dObj2.free();
       }

       // no we have direct values' types
       return type1==type2;
}

Object * CXref::getTrailerEntry(char * name)
{
       Dict * trailer = trailerDict.getDict();

       // lookupNF doesn't create deep copy, so 
       // we have to get value and then make deep copy
       // calling clone method. To keep clean reference counting
       // obj has to be freed
       Object obj;
       trailer->lookupNF(name, &obj);

       Object * retValue=obj.clone();
       obj.free();

       return retValue;
}

Object *CXref::getDocInfo(Object *obj)
{
       // gets object
       Object docObj;
       XRef::getDocInfo(&docObj);

       // creates deep copy and frees object from getDocInfo
       // and initialize parameter from cloned value
       Object * retObj=docObj.clone();
       docObj.free();
       *obj=*retObj;

       return retObj;
}

Object *CXref::getDocInfoNF(Object *obj)
{
       // gets object
       Object docObj;
       XRef::getDocInfoNF(&docObj);

       // creates deep copy and frees object (because getDocInfoNF 
       // uses copy method) from getDocInfo
       // and initialize parameter from cloned value
       Object * retObj=docObj.clone();
       docObj.free();
       // shallow copy of the content (deep copied)
       *obj=*retObj;

       return retObj;
}

Guint CXref::getLastXRefPos() 
{ 
       // TODO can change - when older version is used

       return XRef::getLastXRefPos();
}

Object * CXref::fetch(int num, int gen, Object *obj)
{
       Ref ref={num, gen};

       Object * retObject=changedStorage.get(ref);
       if(retObject)
       {
               // object has been changed
               Object * deepCopy=retObject->clone();

               // shallow copy of content
               // content is deep copy of found object, so
               // this doesn't affect our Object in changedStorage
               *obj=*deepCopy;
               
               // dellocate deepCopy - constructor doesn't
               // destroy content which is copied (shallow)
               // to obj
               delete deepCopy; 
               return obj;
       }

       // delegates to original implementation
       obj=XRef::fetch(num, gen, obj);

       // if object is not null, caches object's deep copy
       /* FIXME uncoment, when cache is ready
       if(cache && obj->getType()!=objNull)
               cache->put(ref, obj->clone());
       */         
       return obj;
}
