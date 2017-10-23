/*
	Copyright (C) 2017 Ramiro Jose Garcia Moraga

	This file is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This file is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with the this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FUKWINDOW_H_
#define FUKWINDOW_H_

typedef struct _FukWindowPrivate FukWindowPrivate;

#define FUK_TYPE_WINDOW	(fuk_window_get_type())
G_DECLARE_FINAL_TYPE(FukWindow,fuk_window,FUK,WINDOW,GtkApplicationWindow)

struct _FukWindowClass
{
  GtkApplicationWindowClass parent_class;
};

struct _FukWindow
{
  GtkApplicationWindow parent_instance;
  FukWindowPrivate * priv;
};

GType		fuk_window_get_type(void);

GtkWidget *	fuk_window_new(void);

#endif /* FUKWINDOW_H_ */
