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

#ifndef FUKGRIDCOLUMN_H_
#define FUKGRIDCOLUMN_H_

typedef enum _FukGridSummaryType FukGridSummaryType;

#define FUK_TYPE_GRID_COLUMN	(fuk_grid_column_get_type())
G_DECLARE_DERIVABLE_TYPE(FukGridColumn,fuk_grid_column,FUK,GRID_COLUMN,GtkTreeViewColumn)

struct _FukGridColumnClass
{
  GtkTreeViewColumnClass parent_class;

  void	   (*option)(FukGridColumn * column);
  gboolean (*filter)(FukGridColumn * column,GtkTreeIter * iter);


};

enum _FukGridSummaryType
{
  FUK_GRID_SUMMARY_NONE,
  FUK_GRID_SUMMARY_GROUP_BY,
  FUK_GRID_SUMMARY_SUM,
  FUK_GRID_SUMMARY_AVG,
  FUK_GRID_SUMMARY_MAX,
  FUK_GRID_SUMMARY_MIN
};

GType	  fuk_grid_column_get_type(void);

FukGridSummaryType
	  fuk_grid_column_get_summary_type(FukGridColumn * column);

void 	  fuk_grid_column_set_summary_type(FukGridColumn * column,
					   FukGridSummaryType type);

guint	  fuk_grid_column_get_data_index(FukGridColumn * column);

void	  fuk_grid_column_set_data_index(FukGridColumn * column,
					   guint data_index);

gboolean  fuk_grid_column_get_editable(FukGridColumn * column);

void	  fuk_grid_column_set_editable(FukGridColumn * column,
				       gboolean setting);



#endif /* FUKGRIDCOLUMN_H_ */
