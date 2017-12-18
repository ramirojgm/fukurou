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

typedef struct _FukInputColumnEditable FukInputColumnEditable;

struct _FukInputPopoverPrivate
{
  GMainLoop *	run_loop;
  gboolean 	result;

  GList *	column_places;

  GtkWidget * 	box_content;
  GtkWidget *	view;
  GtkWidget * 	box_action_area;
  GtkWidget * 	action_bar;
  GtkWidget * 	accept_button;
  GtkWidget * 	cancel_button;
};

struct _FukInputColumnEditable
{
  gint column_index;
  GtkWidget * widget;
};

G_DEFINE_TYPE_WITH_PRIVATE(FukInputPopover,fuk_input_popover,GTK_TYPE_POPOVER)

static void
fuk_input_popover_closed(FukInputPopover * self)
{
  g_main_loop_quit(self->priv->run_loop);
}

static void
fuk_input_popover_button_clicked(FukInputPopover * self,GtkWidget * button)
{
  if(button == self->priv->accept_button)
      self->priv->result = TRUE;
  gtk_widget_hide(GTK_WIDGET(self));
}

static void
fuk_input_popover_init(FukInputPopover * self)
{
  self->priv = fuk_input_popover_get_instance_private(self);
  self->priv->run_loop = g_main_loop_new(NULL,FALSE);
  self->priv->result = FALSE;

  self->priv->box_content = gtk_widget_new(
      GTK_TYPE_BOX,
      "orientation",GTK_ORIENTATION_VERTICAL,
      "visible",TRUE,
      "border-width",5,
      "spacing",5,
      NULL);

  self->priv->view = gtk_widget_new(
      GTK_TYPE_VIEWPORT,
      "visible",TRUE,
      NULL);

  self->priv->box_action_area = gtk_widget_new(
      GTK_TYPE_BOX,
      "orientation",GTK_ORIENTATION_HORIZONTAL,
      "visible",TRUE,
      "border-width",2,
      "spacing",2,
      NULL);

  self->priv->action_bar = gtk_widget_new(
      GTK_TYPE_BUTTON_BOX,
      "layout-style",GTK_BUTTONBOX_CENTER,
      "visible",TRUE,
      NULL);

  self->priv->accept_button = gtk_widget_new(
      GTK_TYPE_BUTTON,
      "label",_("Accept"),
      "visible",TRUE,
      NULL);

  self->priv->cancel_button = gtk_widget_new(
      GTK_TYPE_BUTTON,
      "label",_("Cancel"),
      "visible",TRUE,
      NULL);

  gtk_box_pack_start(
      GTK_BOX(self->priv->action_bar),
      self->priv->accept_button,
      FALSE,
      FALSE,
      0);

  gtk_box_pack_end(
      GTK_BOX(self->priv->action_bar),
      self->priv->cancel_button,
      FALSE,
      FALSE,
      0);

  gtk_box_pack_start(
      GTK_BOX(self->priv->box_content),
      self->priv->view,
      FALSE,
      FALSE,
      0);

  gtk_box_pack_start(
      GTK_BOX(self->priv->box_content),
      self->priv->action_bar,
      FALSE,
      FALSE,
      0);

  gtk_container_add(
      GTK_CONTAINER(self),
      self->priv->box_content);

  gtk_container_add(
      GTK_CONTAINER(self->priv->view),
      self->priv->box_action_area);

  gtk_popover_set_modal(GTK_POPOVER(self),TRUE);

  g_signal_connect(
      G_OBJECT(self),
      "closed",
      G_CALLBACK(fuk_input_popover_closed),
      NULL);

  g_signal_connect_swapped(
      G_OBJECT(self->priv->accept_button),
      "clicked",
      G_CALLBACK(fuk_input_popover_button_clicked),
      self);

  g_signal_connect_swapped(
      G_OBJECT(self->priv->cancel_button),
      "clicked",
      G_CALLBACK(fuk_input_popover_button_clicked),
      self);

  gtk_style_context_add_class(
      gtk_widget_get_style_context(self->priv->action_bar),
      GTK_STYLE_CLASS_LINKED);

  gtk_style_context_add_class(
      gtk_widget_get_style_context(self->priv->view),
      GTK_STYLE_CLASS_VIEW);

  gtk_style_context_add_class(
      gtk_widget_get_style_context(self->priv->accept_button),
      GTK_STYLE_CLASS_SUGGESTED_ACTION);

}

static void
fuk_input_popover_class_init(FukInputPopoverClass * klass)
{

}

FukInputPopover *
fuk_input_popover_new(void)
{
  return FUK_INPUT_POPOVER(gtk_widget_new(FUK_TYPE_INPUT_POPOVER,NULL));
}

GtkWidget *
fuk_input_popover_placeholder_new(
    GtkCellRenderer * render,
    GdkRectangle * rect)
{
  cairo_surface_t * surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,rect->width,rect->height);
  cairo_t * cr = cairo_create(surface);
  GtkWidget * image = gtk_image_new_from_surface(surface);
  gtk_widget_set_size_request(image,rect->width,rect->height);
  gtk_cell_renderer_render(render,
			   cr,
			   image,
			   rect,
			   rect,
			   0);
  cairo_surface_flush(surface);
  cairo_destroy(cr);
  gtk_widget_show(image);
  return image;
}

gboolean
fuk_input_popover_run(FukInputPopover * self,
		      GtkWidget * view,
		      GtkTreeIter * iter)
{
  if(g_main_loop_is_running(self->priv->run_loop))
    return FALSE;

  self->priv->result = FALSE;

  GtkTreeModel * model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));
  GtkTreePath * path = gtk_tree_model_get_path(model,iter);

  GdkRectangle rect = {0,};

  gtk_tree_view_get_cell_area(GTK_TREE_VIEW(view),
			      path,
			      NULL,
			      &rect);
  gtk_tree_view_set_cursor(
      GTK_TREE_VIEW(view),
      path,
      NULL,
      FALSE);

  gtk_popover_set_relative_to(
        GTK_POPOVER(self),
        GTK_WIDGET(view));

  gtk_tree_view_convert_bin_window_to_widget_coords(
      GTK_TREE_VIEW(view),
      rect.x,
      rect.y,
      &(rect.x),
      &(rect.y));

  rect.width = gtk_widget_get_allocated_width(view);

  gtk_popover_set_pointing_to(
      GTK_POPOVER(self),
      &rect);

  GList * cols = gtk_tree_view_get_columns(
      GTK_TREE_VIEW(view));


  guint col_index = 0;
  for(GList * col_iter = g_list_first(cols);col_iter;col_iter = g_list_next(col_iter))
    {
      GtkTreeViewColumn * col = (GtkTreeViewColumn*)col_iter->data;


      GtkWidget * editable = gtk_entry_new();
      gint column_width = gtk_tree_view_column_get_width(col);
      gtk_entry_set_has_frame(GTK_ENTRY(editable),FALSE);
      gtk_entry_set_width_chars(GTK_ENTRY(editable),0);
      gtk_widget_set_size_request(editable,column_width,-1);


      gtk_widget_show(editable);

      gtk_box_pack_start(
	  GTK_BOX(self->priv->box_action_area),
	  editable,
	  FALSE,
	  FALSE,
	  0);

      col_index ++;
    }
  g_list_free(cols);

  gtk_popover_set_position(
      GTK_POPOVER(self),
      GTK_POS_BOTTOM);

  gtk_widget_show(GTK_WIDGET(self));

  g_main_loop_run(self->priv->run_loop);
  return self->priv->result;
}

void
fuk_input_popover_pack_column(FukInputPopover * popover,
			      guint column_index,
			      GtkWidget * editable)
{
  FukInputColumnEditable * col_editable = g_new0(FukInputColumnEditable,1);
  col_editable->column_index = column_index;
  col_editable->widget = GTK_WIDGET(g_object_ref(editable));
  popover->priv->column_places = g_list_append(
      popover->priv->column_places,
      col_editable);
}
