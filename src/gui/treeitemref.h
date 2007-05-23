/*                                                                              
 * PDFedit - free program for PDF document manipulation.                        
 * Copyright (C) 2006, 2007  PDFedit team:      Michal Hocko, 
 *                                              Miroslav Jahoda,       
 *                                              Jozef Misutka, 
 *                                              Martin Petricek                                             
 *
 * Project is hosted on http://sourceforge.net/projects/pdfedit                                                                      
 */ 
#ifndef __TREEITEMREF_H__
#define __TREEITEMREF_H__

#include "treeitem.h"
class QString;

namespace gui {

using namespace pdfobjects;

class TreeData;
class TreeWindow;

class TreeItemObserver;//internal class (observer)

/**
 class holding one CRef object (indirect reference) in tree
 \brief Tree item containing CRef
 */
class TreeItemRef : public TreeItem {
public:
 TreeItemRef(TreeData *_data,Q_ListView *parent,boost::shared_ptr<IProperty> pdfObj,const QString name=QString::null,Q_ListViewItem *after=NULL,const QString &nameId=NULL);
 TreeItemRef(TreeData *_data,Q_ListViewItem *parent,boost::shared_ptr<IProperty> pdfObj,const QString name=QString::null,Q_ListViewItem *after=NULL,const QString &nameId=NULL);
 virtual ~TreeItemRef();
 QString getRef();
 virtual void setOpen(bool open);
 void unOpen();
 bool isComplete();
 //From TreeItemAbstract interface
 virtual bool deepReload(const QString &childName,Q_ListViewItem *oldItem);
 virtual bool validChild(const QString &name,Q_ListViewItem *oldChild);
 virtual ChildType getChildType(const QString &name);
 virtual TreeItemAbstract* createChild(const QString &name,ChildType typ,Q_ListViewItem *after=NULL);
 virtual QStringList getChildNames();
 virtual bool haveChild();
 virtual void reloadSelf();
protected:
 void addData();
private:
 TreeItem* parentCheck();
 /** False, if childs of this items are yet unknown and to be parsed/added, true if child are parsed */
 bool complete;
 /** If this item contains a CRef, this string contain reference target in text form */
 QString selfRef;
};

} // namespace gui

#endif

