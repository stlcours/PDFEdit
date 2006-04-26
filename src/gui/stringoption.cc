/** @file
 StringOption - class for widget containing one editable setting of type string
 Also a base class for editing some more specific types.
 @author Martin Petricek
*/

#include <utils/debug.h>
#include "stringoption.h"
#include <qstring.h>
#include <qlineedit.h>
#include "settings.h"

namespace gui {

/** Default constructor of StringOption item
 @param parent parent Option Editor containing this control
 @param _key Key in settings for this option
 @param defValue Default value if option not found in configuration
 */
StringOption::StringOption(const QString &_key/*=0*/,QWidget *parent/*=0*/,const QString &defValue/*=QString::null*/)
 : Option (_key,parent) {
 ed=new QLineEdit(this,"option_edit");
 if (!defValue.isNull()) ed->setText(defValue);
}

/** default destructor */
StringOption::~StringOption() {
 delete ed;
}

/** write edited value to settings (using key specified in contructor) */
void StringOption::writeValue() {
 globalSettings->write(key,ed->text());
}

/** read value from settings for editing (using key specified in contructor) */
void StringOption::readValue() {
 QString value=globalSettings->read(key);
 if (value.isNull()) return;
 ed->setText(value);
}

/** return size hint of this property editing control
 @return size hint from inner editbox
  */
QSize StringOption::sizeHint() const {
 return ed->sizeHint();
}

/** Called on resizing of property editing control
 Will simply set the same fixed size to inner editbox
 @param e resize event
 */
void StringOption::resizeEvent (QResizeEvent *e) {
 ed->setFixedSize(e->size());
}

} // namespace gui
