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
#include <glib/gi18n.h>

struct _FukAccountPagePrivate
{
  GtkWidget * toolbar;
  GtkWidget * scroll;
  GtkWidget * content;
  GtkWidget * view;

  GtkTreeStore * store;

  GtkTreeViewColumn * col_code,
		    * col_name,
		    * col_description;

  GtkCellRenderer * cell_code,
		  * cell_name,
		  * cell_description;

};

G_DEFINE_TYPE_WITH_PRIVATE(FukAccountPage,fuk_account_page,FUK_TYPE_PAGE)

static void
fuk_account_page_init(FukAccountPage * self)
{
  self->priv = fuk_account_page_get_instance_private(self);

  self->priv->toolbar = gtk_widget_new(
      GTK_TYPE_TOOLBAR,
      "visible",TRUE,
      NULL);

  self->priv->content = gtk_widget_new(
      GTK_TYPE_BOX,
      "orientation",GTK_ORIENTATION_VERTICAL,
      "visible",TRUE,
      NULL);

  self->priv->scroll = gtk_widget_new(
      GTK_TYPE_SCROLLED_WINDOW,
      "shadow-type",GTK_SHADOW_NONE,
      "visible",TRUE,
      NULL);

  self->priv->store = gtk_tree_store_new(4,G_TYPE_INT,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);

  self->priv->view = gtk_widget_new(
      GTK_TYPE_TREE_VIEW,
      "model",self->priv->store,
      "visible",TRUE,
      NULL);

  self->priv->cell_code = gtk_cell_renderer_text_new();
  self->priv->cell_name = gtk_cell_renderer_text_new();
  self->priv->cell_description = gtk_cell_renderer_text_new();

  self->priv->col_code = GTK_TREE_VIEW_COLUMN(g_object_new(
      GTK_TYPE_TREE_VIEW_COLUMN,
      "title",_("Code"),
      "resizable",TRUE,
      "clickable",TRUE,
      NULL));


  self->priv->col_name = GTK_TREE_VIEW_COLUMN(g_object_new(
      GTK_TYPE_TREE_VIEW_COLUMN,
      "title",_("Name"),
      "resizable",TRUE,
      "expand",TRUE,
      "clickable",TRUE,
      NULL));

  self->priv->col_description = GTK_TREE_VIEW_COLUMN(g_object_new(
      GTK_TYPE_TREE_VIEW_COLUMN,
      "title",_("Description"),
      "resizable",TRUE,
      "expand",TRUE,
      "clickable",TRUE,
      NULL));

  gtk_box_pack_start(
      GTK_BOX(self->priv->content),
      self->priv->toolbar,
      FALSE,
      FALSE,
      0);

  gtk_box_pack_end(
      GTK_BOX(self->priv->content),
      self->priv->scroll,
      TRUE,
      TRUE,
      0);

  gtk_tree_view_append_column(
      GTK_TREE_VIEW(self->priv->view),
      self->priv->col_code);

  gtk_tree_view_append_column(
       GTK_TREE_VIEW(self->priv->view),
       self->priv->col_name);

  gtk_tree_view_append_column(
       GTK_TREE_VIEW(self->priv->view),
       self->priv->col_description);



  gtk_container_add(
        GTK_CONTAINER(self->priv->scroll),
        self->priv->view);

  gtk_container_add(
      GTK_CONTAINER(self),
      self->priv->content);
}

static void
fuk_account_page_class_init(FukAccountPageClass * klass)
{

}
