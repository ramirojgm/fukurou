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

#ifndef FUKINPUTPOPOVER_H_
#define FUKINPUTPOPOVER_H_

#define FUK_TYPE_INPUT_POPOVER	(fuk_input_popover_get_type())
G_DECLARE_FINAL_TYPE(FukInputPopover,fuk_input_popover,FUK,INPUT_POPOVER,GtkPopover)

typedef struct _FukInputPopoverPrivate FukInputPopoverPrivate;

struct _FukInputPopoverClass
{
  GtkPopoverClass parent_class;
};

struct _FukInputPopover
{
  GtkPopover parent_instance;
  FukInputPopoverPrivate * priv;
};

FukInputPopover *	fuk_input_popover_new();

void			fuk_input_popover_pack_column(FukInputPopover * popover,
					       guint column_index,
					       GtkWidget * editable);


gboolean		fuk_input_popover_run(FukInputPopover * popover,
					      GtkWidget * view,
					      GtkTreeIter * iter);

#endif /* FUKINPUTPOPOVER_H_ */
