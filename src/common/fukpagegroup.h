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

#ifndef FUKPAGEGROUP_H_
#define FUKPAGEGROUP_H_

typedef struct _FukPageGroupPrivate	FukPageGroupPrivate;
typedef struct _FukPageOptionPrivate	FukPageOptionPrivate;

#define FUK_TYPE_PAGE_GROUP	(fuk_page_group_get_type())
G_DECLARE_FINAL_TYPE(FukPageGroup,fuk_page_group,FUK,PAGE_GROUP,GtkListBoxRow)

#define FUK_TYPE_PAGE_OPTION	(fuk_page_option_get_type())
G_DECLARE_FINAL_TYPE(FukPageOption,fuk_page_option,FUK,PAGE_OPTION,GtkListBoxRow)

struct _FukPageGroupClass
{
  GtkListBoxRowClass parent_class;
};

struct _FukPageGroup
{
  GtkListBoxRow parent_instance;

  FukPageGroupPrivate * priv;
};

struct _FukPageOptionClass
{
  GtkListBoxRowClass parent_class;
};

struct _FukPageOption
{
  GtkListBoxRow parent_instance;
  FukPageOptionPrivate * priv;
};

GType		fuk_page_group_get_type(void);

void		fuk_page_group_add_page_option(FukPageGroup * page_group,GtkWidget * page_option);

void		fuk_page_group_remove_page_option(FukPageGroup * page_group,GtkWidget * page_option);

GType		fuk_page_option_get_type(void);

#endif /* FUKPAGEGROUP_H_ */
