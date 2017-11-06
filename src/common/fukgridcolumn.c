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


#include "../fukurou.h"

typedef struct _FukGridColumnPrivate FukGridColumnPrivate;

struct _FukGridColumnPrivate
{
  FukGridSummaryType summary_type;
  guint    data_index;
  gboolean editable;

};

G_DEFINE_ABSTRACT_TYPE(FukGridColumn,fuk_grid_column,GTK_TYPE_TREE_VIEW_COLUMN)

static void
fuk_grid_column_init(FukGridColumn * self)
{
  FukGridColumnPrivate * priv = fuk_grid_column_get_instance_private(
      self);

  priv->summary_type = FUK_GRID_SUMMARY_NONE;
}

static void
fuk_grid_column_class_init(FukGridColumnClass * self)
{

}
