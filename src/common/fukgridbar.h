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

#ifndef FUKGRIDBAR_H_
#define FUKGRIDBAR_H_

typedef struct _FukGridBarPrivate FukGridBarPrivate;

#define FUK_TYPE_GRID_BAR	(fuk_grid_bar_get_type())
G_DECLARE_FINAL_TYPE(FukGridBar,fuk_grid_bar,FUK,GRID_BAR,GtkViewport)

struct _FukGridBarClass
{
  GtkViewportClass parent_class;
};

struct _FukGridBar
{
  GtkViewport parent_instance;

  FukGridBarPrivate * priv;
};

GType	fuk_grid_bar_get_type(void);


#endif /* FUKGRIDBAR_H_ */
