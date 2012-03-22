/*
 *  rect.h
 *  ancopool
 *
 *  Created by Matthias Klein on 22.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RECT_H
#define RECT_H

namespace GUI {
	class Rect {
	public:
		Rect() {}
		Rect(double left, double right, double top, double bottom) : left(left), right(right), top(top), bottom(bottom) {}
		double left, right, top, bottom;
	};
}

#endif
