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

#ifndef FUKPAGE_H_
#define FUKPAGE_H_

typedef struct _FukPagePrivate FukPagePrivate;

#define FUK_TYPE_PAGE	(fuk_page_get_type())
G_DECLARE_DERIVABLE_TYPE(FukPage,fuk_page,FUK,PAGE,GtkBin)

struct _FukPageClass
{
  GtkBinClass parent_class;
  gboolean (*closing)(FukPage * page);
};


GType		fuk_page_get_type(void);

#endif /* FUKPAGE_H_ */
