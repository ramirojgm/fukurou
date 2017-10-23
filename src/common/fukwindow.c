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

struct _FukWindowPrivate
{
  /* ui */
  GtkWidget * header_bar;
  GtkWidget * menu_button;
  GtkWidget * menu_popover;
  GtkWidget * body;

  GList * pages;
};

G_DEFINE_TYPE_WITH_PRIVATE(FukWindow,fuk_window,GTK_TYPE_APPLICATION_WINDOW)

static void
fuk_window_init(FukWindow * self)
{
  self->priv = fuk_window_get_instance_private(self);

  self->priv->header_bar = gtk_widget_new(
      GTK_TYPE_HEADER_BAR,
      "title","Fukurou",
      "show-close-button", TRUE,
      "visible",TRUE,
      NULL);

  self->priv->body = gtk_widget_new(
      GTK_TYPE_NOTEBOOK,
      "group-name","main-page",
      "show-border",FALSE,
      "scrollable", TRUE,
      "visible",TRUE,
      NULL);

  self->priv->menu_button = gtk_widget_new(
      GTK_TYPE_MENU_BUTTON,
      "visible", TRUE,
      NULL);

  gtk_header_bar_pack_start(
      GTK_HEADER_BAR(self->priv->header_bar),
      self->priv->menu_button);

  gtk_window_set_titlebar(
      GTK_WINDOW(self),self->priv->header_bar);

  gtk_container_add(
      GTK_CONTAINER(self),self->priv->body);
}

static void
fuk_window_class_init(FukWindowClass * klass)
{

}


GtkWidget *
fuk_window_new(void)
{
  return GTK_WIDGET(g_object_new(FUK_TYPE_WINDOW,NULL));
}
