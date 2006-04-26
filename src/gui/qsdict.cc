/** @file
 QObject wrapper around CDict
 Export some functions to scripting.<br>
 See doc/user/scripting.xml or kernel documentation for more informations about these functions
 @author Martin Petricek
*/

#include "qsdict.h"
#include <cobject.h>

namespace gui {

using namespace pdfobjects;

/** Construct wrapper with given CDict */
QSDict::QSDict(boost::shared_ptr<CDict> _dict) : QSIProperty (_dict,"Dict") {
}

/** destructor */
QSDict::~QSDict() {
}

/** get CDict held inside this class. Not exposed to scripting */
boost::shared_ptr<CDict> QSDict::get() {
 return boost::dynamic_pointer_cast<CDict>(obj);
}

/** call CDict::getStringRepresentation(ret); return ret */
QString QSDict::getText() {
 CDict *dict=dynamic_cast<CDict*>(obj.get());
 std::string text;
 dict->getStringRepresentation(text);
 return text;
}

} // namespace gui

//todo: incomplete
