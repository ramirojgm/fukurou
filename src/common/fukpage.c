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

struct _FukPagePrivate
{
  guint padding[5];
};

G_DEFINE_ABSTRACT_TYPE(FukPage,fuk_page,GTK_TYPE_BIN)

static void
fuk_page_init(FukPage * self)
{
  fuk_page_get_instance_private(self);
}

static void
fuk_page_class_init(FukPageClass * self)
{

}

gboolean
fuk_page_closing(FukPage * page)
{
  FukPageClass * klass = FUK_PAGE_GET_CLASS(page);
  if(klass->closing)
    return klass->closing(page);
  else
    return TRUE;
}
