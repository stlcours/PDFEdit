/** @file
 TreeItemDict - class holding one CDict object in tree, descendant of TreeItem
 @author Martin Petricek
*/

#include <cobject.h>
#include "treeitem.h"
#include "treeitemdict.h"
#include "treedata.h"
#include "treewindow.h"
#include "util.h"
#include "pdfutil.h"

namespace gui {

using namespace std;
using namespace util;

/**
 @copydoc TreeItem(TreeData *,QListView *,IProperty *,const QString,QListViewItem *)
 */
TreeItemDict::TreeItemDict(TreeData *_data,QListView *parent,IProperty *pdfObj,const QString name/*=QString::null*/,QListViewItem *after/*=NULL*/):TreeItem(_data,parent,pdfObj,name,after) {
 assert(data);
 reload(false);
 initObserver();
}

/**
@copydoc TreeItem(TreeData *,QListViewItem *,IProperty *,const QString,QListViewItem *)
 */
TreeItemDict::TreeItemDict(TreeData *_data,QListViewItem *parent,IProperty *pdfObj,const QString name/*=QString::null*/,QListViewItem *after/*=NULL*/):TreeItem(_data,parent,pdfObj,name,after) {
 assert(data);
 reload(false);
 initObserver();
}

//See TreeItemAbstract for description of this virtual method
TreeItemAbstract* TreeItemDict::createChild(const QString &name,ChildType typ,QListViewItem *after/*=NULL*/) {
 CDict *dict=dynamic_cast<CDict*>(obj);
 boost::shared_ptr<IProperty> property=dict->getProperty(name);
 return TreeItem::create(data,this,property.get(),name,after);
}

//See TreeItemAbstract for description of this virtual method
ChildType TreeItemDict::getChildType(const QString &name) {
 CDict *dict=dynamic_cast<CDict*>(obj);
 boost::shared_ptr<IProperty> property=dict->getProperty(name);
 return property->getType();
}

//See TreeItemAbstract for description of this virtual method
QStringList TreeItemDict::getChildNames() {
 QStringList itemList;
 CDict *dict=dynamic_cast<CDict*>(obj);
 vector<string> list;
 dict->getAllPropertyNames(list);
 vector<string>::iterator it;
 for( it=list.begin();it!=list.end();++it) { // for each property
  boost::shared_ptr<IProperty> property=dict->getProperty(*it);
  if (!data->showSimple() && isSimple(property)) continue; //simple item -> skip it
  itemList += *it;
 }
 return itemList;
}

/** default destructor */
TreeItemDict::~TreeItemDict() {
 uninitObserver();
}

} // namespace gui
