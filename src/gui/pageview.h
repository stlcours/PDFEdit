#ifndef __PAGEVIEW_H__
#define __PAGEVIEW_H__

#include <qlabel.h>
#include <qevent.h>
#include <qpainter.h>

/** QWidget's class for viewing a page.
 *  
 */
class PageView : public QLabel {
	Q_OBJECT
	public:
/** Enum type for setting selection mode.
 * RectSelection	 =  the selected area will be bounded thin rectangle
 * FillRectSelection	 =  the selected area will be filled rectangle
 * RectFillRectSelection =  moving or creating selected area -> FillRectSelection
 * 			 =  otherwise -> RectSelection
 * FillRectRectSelection =  moving or creating selected area -> RectSelection
 * 			 =  otherwise -> FillRectSelection
 */
		enum SelectionMode { RectSelection, FillRectSelection, RectFillRectSelection, FillRectRectSelection };

/** Default constructor of pageView.
 * @param parent widget containing this control
 *
 * It sets 
 *	selection mode to FillRectRectSelection
 **/
		PageView (QWidget *parent);
/** Deconstructor of pageView.
 */
		virtual ~PageView ();
/** Method for escape from creating or moving selected area.
 *
 * @return  if was terminated creating or moving process, return true
 */
		bool escapeSelection ();	// return true if escaped from selection or moving mode
	signals:
/** Signal generated by selection process after the selected area
 * @param  the selected area
 */
		void leftClicked ( const QRect & );
/** Signal generated by mouse right release event
 * @param  global position of mouse right release event
 * @param  contains rectangle of the selected area on the page (if mouse event is out of selected area on the page,
 * 		than parameter will be contains  position (rectangle [x,y,x,y]) mouse event on the page)
 */
		void rightClicked ( const QPoint &, const QRect & );
/** Signal generated by selection process after move the selected area
 * @param  left top position of moved selected area on the page
 */
		void selectionMovedTo ( const QPoint & );		// top left
        protected:
/** Method for process mouse press events (see QWidget's method).
 * @param  press event
 *
 * Start creating or moving process with selected area.
 */
		virtual void mousePressEvent ( QMouseEvent * e );
/** Method for process mouse release events (see QWidget's method).
 * @param  release event
 *
 * Finish creating or moving process with selected area.
 *   ( emits leftClicked, rightClicked, selectionMovedTo signals )
 */
		virtual void mouseReleaseEvent ( QMouseEvent * e );

//		virtual void mouseDoubleClickEvent ( QMouseEvent * e );

/** Method for process mouse move events (see QWidget's method).
 * @param  move event
 *
 * If is started creating or moving process, process changes.
 */
		virtual void mouseMoveEvent ( QMouseEvent * e );
	public slots:
/** Method set selection mode.
 * @param selection mode (see 'enum SelectionMode')
 *
 * Selection mode is dafault sets to FillRectRectSelection .
 */
		void setSelectionMode ( enum SelectionMode m );		// default FillRectRectSelection
/** Method set selected area.
 * @param rectangle to select area on page
 */
		void setSelectedRect ( QRect & newRect );
/** Method unset selected area
 */
		void unSelect ( );
/** Method set pixmap image of page.
 * @param image of page as QPixmap
 */
		virtual void setPixmap ( const QPixmap & qp );
	private:
/** enum of selection variant
 * 	KeepSelection	= kepp actual selection variant 
 */
		enum SelectionSet { KeepSelection, IsSelected, IsSelecting };
/** Method for change selection variant
 * @param selection variant (see enum SelectionSet)
 */
		void changeSelection ( enum SelectionSet );
/** Method for undraw selected area on the page
 */
		void unDrawSelection ( void );
/** Method for draw rectangle on page.
 * @param rectangle to draw
 * @param if rectangle have been draw or undraw
 *
 * See also: changeSelection, setSelectionMode.
 */
		void drawRect ( QRect * newRect, bool unDraw = false );
/** Method for undraw old rectangle and draw new rectangle on page.
 * @param old rectangle to undraw
 * @param new rectangle to draw
 * @param selection variant to draw new rectangle
 */
		void drawRect ( QRect * oldRect, QRect * newRect, enum SelectionSet ss = KeepSelection );
/** Method for redraw old rectangle to new rectangle on page.
 * @param old rectangle to undraw
 * @param point of right bottom point of new rectangle to draw (left top point is same as have old rectangle)
 */
		void drawRect ( QRect * oldRect, const QPoint & toPoint );
	private:
//		QPixmap * pageImage;

/** temporary selected area
 */
		QRect * mouseRectSelected;	// when rectSelected is not NULL than mouseRectSelected is not NULL
/** selected area
 *
 * Must be normalized rectangle.
 */
		QRect * rectSelected;
/** is true if selected area is moving or creating new
 */
		bool	isPress;
/** is true if selected area is moving
 */
		bool	isMoving;
/** is true if is set 'IsSelecting' selection variant
 */
		bool	quickSelection;
/** selection mode which is set
 */
		enum SelectionMode selectionMode;
/** point is relative position mouse cursor from left top selected rectangle which is moving
 */
		QPoint	pointInRect;
};

#endif
