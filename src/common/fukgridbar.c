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

struct _FukGridBarPrivate
{
  GtkWidget * box_main;
  GtkWidget * tb_first;
  GtkWidget * tb_previous;
  GtkWidget * ent_position;
  GtkWidget * tb_next;
  GtkWidget * tb_last;
  GtkWidget * box_position;

  GtkWidget * tb_add;
  GtkWidget * tb_remove;
  GtkWidget * tb_edit;
  GtkWidget * tb_update;
  GtkWidget * box_action;

  GtkTreeSelection * ts_selection;
  gulong	     ts_selection_changed;
};

G_DEFINE_TYPE_WITH_PRIVATE(FukGridBar,fuk_grid_bar,GTK_TYPE_VIEWPORT)

enum FukGridBarSignal
{
  FUK_GRID_BAR_SIGNAL_ADD_ROW,
  FUK_GRID_BAR_SIGNAL_REMOVE_ROW,
  FUK_GRID_BAR_SIGNAL_EDIT_ROW,
  FUK_GRID_BAR_SIGNAL_UPDATE_ALL,
  FUK_GRID_BAR_SIGNAL_N,
};

enum FukGridBarProp
{
  FUK_GRID_BAR_PROP_SELECTION = 1,
  FUK_GRID_BAR_PROP_N,
};

static guint
_fuk_grid_bar_signals[FUK_GRID_BAR_SIGNAL_N] = {0,};

static GParamSpec *
_fuk_grid_bar_properties[FUK_GRID_BAR_PROP_N] = {0,};

static void
_fuk_grid_bar_on_tb_add_clicked(FukGridBar * bar);

static void
_fuk_grid_bar_on_tb_remove_clicked(FukGridBar * bar);

static void
_fuk_grid_bar_on_tb_edit_clicked(FukGridBar * bar);

static void
_fuk_grid_bar_on_tb_update_clicked(FukGridBar * bar);


static void
fuk_grid_bar_init(FukGridBar * self)
{
  self->priv = fuk_grid_bar_get_instance_private(self);

  gtk_viewport_set_shadow_type(GTK_VIEWPORT(self),GTK_SHADOW_NONE);

  self->priv->box_main = gtk_widget_new(
      GTK_TYPE_BOX,
      "orientation",GTK_ORIENTATION_HORIZONTAL,
      "visible",TRUE,
      "spacing",5,
      "border-width",5,
      NULL);

  self->priv->box_position = gtk_widget_new(
      GTK_TYPE_BOX,
      "orientation",GTK_ORIENTATION_HORIZONTAL,
      "visible",TRUE,
      "sensitive",FALSE,
      NULL);

  self->priv->box_action = gtk_widget_new(
      GTK_TYPE_BOX,
      "orientation",GTK_ORIENTATION_HORIZONTAL,
      "visible",TRUE,
      "sensitive",FALSE,
      NULL);

  self->priv->tb_first = gtk_widget_new(
      GTK_TYPE_BUTTON,
      "image", gtk_widget_new(GTK_TYPE_IMAGE,"icon-name", "go-first-symbolic", "icon-size",GTK_ICON_SIZE_MENU,NULL),
      "visible",TRUE,
      NULL);

  self->priv->tb_previous = gtk_widget_new(
      GTK_TYPE_BUTTON,
      "image", gtk_widget_new(GTK_TYPE_IMAGE,"icon-name", "go-previous-symbolic", "icon-size",GTK_ICON_SIZE_MENU,NULL),
      "visible",TRUE,
      NULL);

  self->priv->ent_position = gtk_widget_new(
      GTK_TYPE_ENTRY,
      "placeholder-text", "0 de 0",
      "xalign",0.5,
      "width-chars",10,
      "visible",TRUE,
      NULL);

  self->priv->tb_next = gtk_widget_new(
      GTK_TYPE_BUTTON,
      "image", gtk_widget_new(GTK_TYPE_IMAGE,"icon-name", "go-next-symbolic", "icon-size",GTK_ICON_SIZE_MENU,NULL),
      "visible",TRUE,
      NULL);

  self->priv->tb_last = gtk_widget_new(
      GTK_TYPE_BUTTON,
      "image", gtk_widget_new(GTK_TYPE_IMAGE,"icon-name", "go-last-symbolic", "icon-size",GTK_ICON_SIZE_MENU,NULL),
      "visible",TRUE,
      NULL);

  self->priv->tb_add = gtk_widget_new(
      GTK_TYPE_BUTTON,
      "image", gtk_widget_new(GTK_TYPE_IMAGE,"icon-name", "list-add-symbolic", "icon-size",GTK_ICON_SIZE_MENU,NULL),
      "visible",TRUE,
      NULL);

  self->priv->tb_remove = gtk_widget_new(
      GTK_TYPE_BUTTON,
      "image", gtk_widget_new(GTK_TYPE_IMAGE,"icon-name", "list-remove-symbolic", "icon-size",GTK_ICON_SIZE_MENU,NULL),
      "visible",TRUE,
      NULL);

  self->priv->tb_edit = gtk_widget_new(
      GTK_TYPE_BUTTON,
      "image", gtk_widget_new(GTK_TYPE_IMAGE,"icon-name", "document-edit-symbolic", "icon-size",GTK_ICON_SIZE_MENU,NULL),
      "visible",TRUE,
      NULL);

  self->priv->tb_update = gtk_widget_new(
      GTK_TYPE_BUTTON,
      "image", gtk_widget_new(GTK_TYPE_IMAGE,"icon-name", "view-refresh-symbolic", "icon-size",GTK_ICON_SIZE_MENU,NULL),
      "visible",TRUE,
      NULL);

  gtk_box_pack_start(GTK_BOX(self->priv->box_position),
		     self->priv->tb_first,
		     FALSE,
		     FALSE,
		     0);

  gtk_box_pack_start(GTK_BOX(self->priv->box_position),
  		     self->priv->tb_previous,
  		     FALSE,
  		     FALSE,
  		     0);

  gtk_box_pack_start(GTK_BOX(self->priv->box_position),
  		     self->priv->ent_position,
  		     FALSE,
  		     FALSE,
  		     0);

  gtk_box_pack_start(GTK_BOX(self->priv->box_position),
  		     self->priv->tb_next,
  		     FALSE,
  		     FALSE,
  		     0);

  gtk_box_pack_start(GTK_BOX(self->priv->box_position),
  		     self->priv->tb_last,
  		     FALSE,
  		     FALSE,
  		     0);

  gtk_box_pack_start(GTK_BOX(self->priv->box_action),
   		     self->priv->tb_add,
   		     FALSE,
   		     FALSE,
   		     0);

  gtk_box_pack_start(GTK_BOX(self->priv->box_action),
     		     self->priv->tb_remove,
     		     FALSE,
     		     FALSE,
     		     0);

  gtk_box_pack_start(GTK_BOX(self->priv->box_action),
     		     self->priv->tb_edit,
     		     FALSE,
     		     FALSE,
     		     0);

  gtk_box_pack_start(GTK_BOX(self->priv->box_action),
     		     self->priv->tb_update,
     		     FALSE,
     		     FALSE,
     		     0);

  gtk_box_pack_start(GTK_BOX(self->priv->box_main),
  		     self->priv->box_position,
  		     FALSE,
  		     FALSE,
  		     0);

  gtk_box_pack_start(GTK_BOX(self->priv->box_main),
    		     self->priv->box_action,
    		     FALSE,
    		     FALSE,
    		     0);

  gtk_container_add(GTK_CONTAINER(self),
		    self->priv->box_main);

  g_signal_connect_swapped(
      G_OBJECT(self->priv->tb_add),
      "clicked",
      G_CALLBACK(_fuk_grid_bar_on_tb_add_clicked),
      self);

  g_signal_connect_swapped(
      G_OBJECT(self->priv->tb_remove),
      "clicked",
      G_CALLBACK(_fuk_grid_bar_on_tb_remove_clicked),
      self);

  g_signal_connect_swapped(
      G_OBJECT(self->priv->tb_edit),
      "clicked",
      G_CALLBACK(_fuk_grid_bar_on_tb_edit_clicked),
      self);

  g_signal_connect_swapped(
      G_OBJECT(self->priv->tb_update),
      "clicked",
      G_CALLBACK(_fuk_grid_bar_on_tb_update_clicked),
      self);

  gtk_style_context_add_class(
      gtk_widget_get_style_context(GTK_WIDGET(self)),
      GTK_STYLE_CLASS_BACKGROUND);

  gtk_style_context_add_class(
      gtk_widget_get_style_context(self->priv->box_position),
      GTK_STYLE_CLASS_LINKED);

  gtk_style_context_add_class(
      gtk_widget_get_style_context(self->priv->box_action),
      GTK_STYLE_CLASS_LINKED);
}


static void
fuk_grid_bar_update_button(FukGridBar * self)
{
  if(self->priv->ts_selection)
    {
      GtkTreeIter iter;
      GtkTreeModel * model = gtk_tree_view_get_model(
	  gtk_tree_selection_get_tree_view(
	      self->priv->ts_selection));

      if(model)
	{
	  if(gtk_tree_selection_get_selected(
		  self->priv->ts_selection,
		  NULL,
		  &iter))
	    {
	      GtkTreePath * path = gtk_tree_model_get_path(
		  model,
		  &iter);

	      gint depth = 0;
	      gint * indices = gtk_tree_path_get_indices_with_depth(
		  path,
		  &depth);

	      guint index = indices[depth - 1];

	      GtkTreeIter * parent_iter = NULL;
	      if(gtk_tree_path_up(path) && depth > 1)
		{
		  gtk_tree_model_get_iter(model,&iter,path);
		  parent_iter = &iter;
		}
	      gtk_tree_path_free(path);

	      guint count = gtk_tree_model_iter_n_children(
		  model,
		  parent_iter);

	      gchar * position = g_strdup_printf(
		  _("%d of %d"),
		  index + 1,
		  count);

	      gtk_entry_set_placeholder_text(
		  GTK_ENTRY(self->priv->ent_position),
		  position);

	      g_free(position);

	      gtk_widget_set_sensitive(
		  GTK_WIDGET(self->priv->box_action),
		  TRUE);

	      gtk_widget_set_sensitive(
		  GTK_WIDGET(self->priv->box_position),
		  TRUE);

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_first),
		index > 0);

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_previous),
		index > 0);

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_next),
		index < (count - 1));

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_last),
		index < (count - 1));

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_remove),
		TRUE);

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_edit),
		TRUE);
	    }
	  else
	    {
	      guint count = gtk_tree_model_iter_n_children(
		  model,
		  NULL);

	      gchar * position = g_strdup_printf(
		  _("%d"),
		  count);

	      gtk_entry_set_placeholder_text(
		  GTK_ENTRY(self->priv->ent_position),
		  position);

	      g_free(position);

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_first),
		FALSE);

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_previous),
		FALSE);

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_next),
		FALSE);

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_last),
		FALSE);

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_remove),
		FALSE);

	      gtk_widget_set_sensitive(
		GTK_WIDGET(self->priv->tb_edit),
		FALSE);
	    }

	  gtk_widget_set_sensitive(
	 	  GTK_WIDGET(self->priv->box_action),
	 	  TRUE);

	  gtk_widget_set_sensitive(
		  GTK_WIDGET(self->priv->box_position),
		  TRUE);

	}
      else
	{
	  gtk_entry_set_placeholder_text(
	      GTK_ENTRY(self->priv->ent_position),
	      _("Empty"));

	  gtk_widget_set_sensitive(
	    GTK_WIDGET(self->priv->box_action),
	    FALSE);

	  gtk_widget_set_sensitive(
	    GTK_WIDGET(self->priv->box_position),
	    FALSE);
	}
    }
  else
    {
      gtk_entry_set_placeholder_text(
	  GTK_ENTRY(self->priv->ent_position),
	  _("Empty"));

      gtk_widget_set_sensitive(
	  GTK_WIDGET(self->priv->box_action),
	  FALSE);

      gtk_widget_set_sensitive(
      	  GTK_WIDGET(self->priv->box_position),
      	  FALSE);
    }
}


static void
fuk_grid_bar_selection_changed(FukGridBar * self,GtkTreeSelection * selection)
{
  g_print("changed");
  fuk_grid_bar_update_button(FUK_GRID_BAR(self));
}


static void
fuk_grid_bar_constructed(GObject * self)
{
 fuk_grid_bar_update_button(FUK_GRID_BAR(self));
}

static void
fuk_grid_bar_set_property(GObject * object,
			  guint property_id,
			  const GValue * src,
			  GParamSpec * param)
{
  FukGridBarPrivate * priv = FUK_GRID_BAR(object)->priv;

  switch(property_id)
  {
    case FUK_GRID_BAR_PROP_SELECTION:
      {
	gpointer selection = g_value_get_object(src);
	if(priv->ts_selection_changed)
	  {
	    g_signal_handler_disconnect(priv->ts_selection,priv->ts_selection_changed);
	    g_clear_object(&(priv->ts_selection));
	    priv->ts_selection_changed = 0;
	  }
	if(selection)
	  {
	    priv->ts_selection = g_object_ref(selection);
	    priv->ts_selection_changed = g_signal_connect_swapped(
		selection,
		"changed",
		G_CALLBACK(fuk_grid_bar_selection_changed),
		object);
	  }
	fuk_grid_bar_update_button(FUK_GRID_BAR(object));
      }
      break;
  }
}

static void
fuk_grid_bar_get_property(GObject * object,
			  guint property_id,
			  GValue * dest,
			  GParamSpec * param)
{
  FukGridBarPrivate * priv = FUK_GRID_BAR(object)->priv;

  switch(property_id)
  {
    case FUK_GRID_BAR_PROP_SELECTION:
      g_value_set_object(dest,priv->ts_selection);
      break;
  }
}


static void
fuk_grid_bar_class_init(FukGridBarClass * klass)
{
  G_OBJECT_CLASS(klass)->constructed = fuk_grid_bar_constructed;

  G_OBJECT_CLASS(klass)->set_property = fuk_grid_bar_set_property;
  G_OBJECT_CLASS(klass)->get_property = fuk_grid_bar_get_property;

  GType param_types[2] = {0,};
  param_types[0] = GTK_TYPE_TREE_ITER;

  _fuk_grid_bar_properties[FUK_GRID_BAR_PROP_SELECTION] = g_param_spec_object(
      "selection",
      _("Selection"),
      _("Selection of GtkTreeView"),
      GTK_TYPE_TREE_SELECTION,
      G_PARAM_READWRITE);

  _fuk_grid_bar_signals[FUK_GRID_BAR_SIGNAL_ADD_ROW] = g_signal_newv(
      "add-row",
      G_TYPE_FROM_CLASS(klass),
      G_SIGNAL_NO_HOOKS|G_SIGNAL_NO_RECURSE|G_SIGNAL_RUN_LAST,
      NULL,
      NULL,
      NULL,
      NULL,
      G_TYPE_NONE,
      1,
      param_types);

  _fuk_grid_bar_signals[FUK_GRID_BAR_SIGNAL_REMOVE_ROW] = g_signal_newv(
      "remove-row",
      G_TYPE_FROM_CLASS(klass),
      G_SIGNAL_NO_HOOKS|G_SIGNAL_NO_RECURSE|G_SIGNAL_RUN_LAST,
      NULL,
      NULL,
      NULL,
      NULL,
      G_TYPE_NONE,
      1,
      param_types);

  _fuk_grid_bar_signals[FUK_GRID_BAR_SIGNAL_EDIT_ROW] = g_signal_newv(
      "edit-row",
      G_TYPE_FROM_CLASS(klass),
      G_SIGNAL_NO_HOOKS|G_SIGNAL_NO_RECURSE|G_SIGNAL_RUN_LAST,
      NULL,
      NULL,
      NULL,
      NULL,
      G_TYPE_NONE,
      1,
      param_types);

  _fuk_grid_bar_signals[FUK_GRID_BAR_SIGNAL_UPDATE_ALL] = g_signal_newv(
      "update-all",
      G_TYPE_FROM_CLASS(klass),
      G_SIGNAL_NO_HOOKS|G_SIGNAL_NO_RECURSE|G_SIGNAL_RUN_LAST,
      NULL,
      NULL,
      NULL,
      NULL,
      G_TYPE_NONE,
      1,
      param_types);

  g_object_class_install_properties(G_OBJECT_CLASS(klass),
				    FUK_GRID_BAR_PROP_N,
				    _fuk_grid_bar_properties);
}

static void
_fuk_grid_bar_on_tb_add_clicked(FukGridBar * self)
{
  GtkTreeIter iter;
  if(gtk_tree_selection_get_selected(
	  self->priv->ts_selection,
	  NULL,
	  &iter))
    {
      g_signal_emit(
	self,
	_fuk_grid_bar_signals[FUK_GRID_BAR_SIGNAL_ADD_ROW],
	0,
	&iter);
    }
  else
    {
      g_signal_emit(
	self,
	_fuk_grid_bar_signals[FUK_GRID_BAR_SIGNAL_ADD_ROW],
	0,
	NULL);
    }
}

static void
_fuk_grid_bar_on_tb_remove_clicked(FukGridBar * self)
{
  GtkTreeIter iter;
  if(gtk_tree_selection_get_selected(
	  self->priv->ts_selection,
	  NULL,
	  &iter))
  {
    g_signal_emit(
      self,
      _fuk_grid_bar_signals[FUK_GRID_BAR_SIGNAL_REMOVE_ROW],
      0,
      &iter);
  }
}

static void
_fuk_grid_bar_on_tb_edit_clicked(FukGridBar * self)
{
  GtkTreeIter iter;
  if(gtk_tree_selection_get_selected(
	  self->priv->ts_selection,
	  NULL,
	  &iter))
  {
    g_signal_emit(
      self,
      _fuk_grid_bar_signals[FUK_GRID_BAR_SIGNAL_EDIT_ROW],
      0,
      &iter);
  }
}

static void
_fuk_grid_bar_on_tb_update_clicked(FukGridBar * self)
{
  GtkTreeIter iter;
  if(gtk_tree_selection_get_selected(
	  self->priv->ts_selection,
	  NULL,
	  &iter))
    {
      g_signal_emit(
	self,
	_fuk_grid_bar_signals[FUK_GRID_BAR_SIGNAL_UPDATE_ALL],
	0,
	&iter);
    }
  else
    {
      g_signal_emit(
	self,
	_fuk_grid_bar_signals[FUK_GRID_BAR_SIGNAL_UPDATE_ALL],
	0,
	NULL);
    }
}
