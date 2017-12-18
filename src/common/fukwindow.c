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
  //GtkWidget * menu_button;
  //GtkWidget * menu_popover;
  GtkWidget * paned;
  GtkWidget * menu_viewport;
  GtkWidget * menu_box;
  GtkWidget * body;
  GList * pages;

};

G_DEFINE_TYPE_WITH_PRIVATE(FukWindow,fuk_window,GTK_TYPE_APPLICATION_WINDOW)

static void fuk_window_menu_init(FukWindow * win);

gboolean fuk_page_closing(FukPage * page);

static gboolean
fuk_window_secondary_delete_event(FukWindow * win,
				  GdkEvent * event,
				  GtkWidget * secondary_win)
{
  gboolean cancel = FALSE;

  GtkNotebook * notebook = GTK_NOTEBOOK(gtk_bin_get_child(
	  GTK_BIN(secondary_win)));

  g_object_set_data(G_OBJECT(notebook),"deleting-window",GINT_TO_POINTER(1));

  GList * pages = gtk_container_get_children(
      GTK_CONTAINER(notebook));

  for(GList * page_iter = g_list_first(pages);page_iter;page_iter = g_list_next(page_iter))
    {
      gboolean close = fuk_page_closing(FUK_PAGE(page_iter->data));
      if(close)
	{
	  win->priv->pages = g_list_remove(win->priv->pages,page_iter->data);
	  gtk_notebook_detach_tab(notebook,GTK_WIDGET(page_iter->data));
	  g_object_unref(page_iter->data);
	}
      else
	{
	  cancel = TRUE;
	}
    }
  g_list_free(pages);
  g_object_set_data(G_OBJECT(notebook),"deleting-window",GINT_TO_POINTER(0));
  return cancel;
}

static void
fuk_window_secondary_page_removed (FukWindow * win,
				   GtkWidget   *child,
				   guint        page_num,
				   GtkNotebook *notebook)
{
  gboolean deleting_window = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(notebook),"deleting-window"));
  if(!deleting_window)
    {
      if(gtk_notebook_get_n_pages(notebook) == 0)
	{
	  GtkWidget * toplevel = gtk_widget_get_toplevel(
	      GTK_WIDGET(notebook));

	  gtk_widget_destroy(toplevel);
	}
    }
}

static void
fuk_window_page_close(FukWindow * window,
		      GtkButton * button)
{
  GtkWidget * page = GTK_WIDGET(
      g_object_get_data(
	  G_OBJECT(button),
	  "page"));

  if(fuk_page_closing(FUK_PAGE(page)))
    {
      GtkNotebook * notebook = GTK_NOTEBOOK(gtk_widget_get_parent(
	  page));
      window->priv->pages = g_list_remove(window->priv->pages,page);
      gtk_notebook_detach_tab(notebook,page);
      g_object_unref(page);
    }
}

static GtkNotebook *
fuk_window_body_create_window(FukWindow * win,
			      GtkWidget * page,
			      gint x,
			      gint y,
			      GtkWidget * body)
{
  GtkWidget * window = gtk_widget_new(
      GTK_TYPE_WINDOW,
      "title","Fukurou",
      NULL);

  GtkWidget * notebook = gtk_widget_new(
      GTK_TYPE_NOTEBOOK,
      "group-name","main-page",
      "show-border",FALSE,
      "scrollable", TRUE,
      "visible",TRUE,
      NULL);

  gtk_container_add(GTK_CONTAINER(window),notebook);

  g_signal_connect_swapped(
      G_OBJECT(window),
      "delete-event",
      G_CALLBACK(fuk_window_secondary_delete_event),
      win);

  g_signal_connect_swapped(
      G_OBJECT(notebook),
      "page-removed",
      G_CALLBACK(fuk_window_secondary_page_removed),
      win);

  gtk_window_move(GTK_WINDOW(window),x,y);
  gtk_widget_show(window);
  return GTK_NOTEBOOK(notebook);
}

static void
fuk_window_menu_row_activated(FukWindow * win,
			      GtkListBoxRow * row,
			      GtkListBox * object)
{
  if(FUK_IS_PAGE_GROUP(row))
    {
      g_signal_emit_by_name(row,"toggle",NULL);
    }
}

static void
fuk_window_page_option_clicked(FukWindow * win,
			       FukPageOption * option)
{
  GValue page_type = G_VALUE_INIT;
  g_value_init(&page_type,G_TYPE_GTYPE);
  g_object_get_property(G_OBJECT(option),"page-type",&page_type);
  GType gtype = g_value_get_gtype(&page_type);
  g_value_unset(&page_type);
  GtkWidget * page = NULL;
  GList * pages = win->priv->pages;
  for(GList * page_iter = g_list_first(pages);page_iter;page_iter = g_list_next(page_iter))
    {
      if(G_TYPE_FROM_INSTANCE(page_iter->data) == gtype)
	{
	  page = page_iter->data;
	  break;
	}
    }

  if(!page)
    {
      gchar * icon_name;
      gchar * title;

      g_object_get(
	  G_OBJECT(option),
	  "icon-name",&icon_name,
	  "title",&title,
	  NULL);

      GtkWidget * label_icon = gtk_widget_new(
	  GTK_TYPE_IMAGE,
	  "icon-name",icon_name,
	  "icon-size",GTK_ICON_SIZE_MENU,
	  "visible",TRUE,
	  NULL);

      GtkWidget * label_title = gtk_widget_new(
	  GTK_TYPE_LABEL,
	  "label",title,
	  "visible",TRUE,
	  NULL);

      GtkWidget * label_button = gtk_widget_new(
	  GTK_TYPE_BUTTON,
	  "relief",GTK_RELIEF_NONE,
	  "visible", TRUE,
	  NULL);

      GtkWidget * label_widget = gtk_widget_new(
	  GTK_TYPE_BOX,
	  "orientation",GTK_ORIENTATION_HORIZONTAL,
	  "spacing",5,
	  "visible",TRUE,
	  NULL);

      gtk_container_add(
	  GTK_CONTAINER(label_button),
	  gtk_widget_new(
	      GTK_TYPE_IMAGE,
	      "icon-name","window-close-symbolic",
	      "icon-size",GTK_ICON_SIZE_MENU,
	      "visible",TRUE,
	      NULL));

      gtk_box_pack_start(
	  GTK_BOX(label_widget),
	  label_icon,
	  FALSE,
	  FALSE,
	  0);

      gtk_box_pack_start(
      	  GTK_BOX(label_widget),
      	  label_title,
      	  TRUE,
      	  TRUE,
      	  0);

      gtk_box_pack_end(
      	  GTK_BOX(label_widget),
      	  label_button,
      	  FALSE,
      	  FALSE,
      	  0);

      page = gtk_widget_new(
	  gtype,
	  "visible",TRUE,
	  NULL);

      gtk_notebook_append_page(
	  GTK_NOTEBOOK(win->priv->body),
	  page,label_widget);

      gtk_notebook_set_tab_reorderable(
	  GTK_NOTEBOOK(win->priv->body),
	  page,
	  TRUE);

      gtk_notebook_set_tab_detachable(
	  GTK_NOTEBOOK(win->priv->body),
	  page,
	  TRUE);

      g_object_set_data(
	  G_OBJECT(label_button),
	  "page",
	  page);

      g_signal_connect_swapped(
	  G_OBJECT(label_button),
	  "clicked",
	  G_CALLBACK(fuk_window_page_close),
	  win);

      g_free(icon_name);
      g_free(title);
      win->priv->pages = g_list_append(win->priv->pages,g_object_ref(page));
    }

  GtkWidget * page_notebook = gtk_widget_get_parent(page);

  gtk_notebook_set_current_page(
      GTK_NOTEBOOK(page_notebook),
      gtk_notebook_page_num(
	  GTK_NOTEBOOK(page_notebook),
	  page));

  gtk_window_present(GTK_WINDOW(
      gtk_widget_get_toplevel(
	  page_notebook)));

}

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

  self->priv->paned = gtk_widget_new(
     GTK_TYPE_PANED,
     "orientation", GTK_ORIENTATION_HORIZONTAL,
     "visible",TRUE,
     NULL);

  self->priv->body = gtk_widget_new(
      GTK_TYPE_NOTEBOOK,
      "group-name","main-page",
      "show-border",FALSE,
      "scrollable", TRUE,
      "visible",TRUE,
      NULL);

  self->priv->menu_viewport = gtk_widget_new(
      GTK_TYPE_VIEWPORT,
      "shadow-type",GTK_SHADOW_NONE,
      "visible", TRUE,
      NULL);

  self->priv->menu_box = gtk_widget_new(
      GTK_TYPE_LIST_BOX,
      "activate-on-single-click",TRUE,
      "visible", TRUE,
      NULL);

  g_signal_connect_swapped(
      self->priv->menu_box,
      "row-activated",
      fuk_window_menu_row_activated,
      self);

  gtk_window_set_default_size(GTK_WINDOW(self),800,600);

  gtk_window_set_titlebar(
      GTK_WINDOW(self),
      self->priv->header_bar);

  gtk_container_add(
      GTK_CONTAINER(self),
      self->priv->paned);

  gtk_paned_pack1(
      GTK_PANED(self->priv->paned),
      self->priv->menu_viewport,
      FALSE,
      FALSE);

  gtk_paned_pack2(
      GTK_PANED(self->priv->paned),
      self->priv->body,
      TRUE,
      TRUE);

  gtk_container_add(
      GTK_CONTAINER(self->priv->menu_viewport),
      self->priv->menu_box);


  g_signal_connect_swapped(
      G_OBJECT(self->priv->body),
      "create-window",
      G_CALLBACK(fuk_window_body_create_window),
      self);

  gtk_style_context_add_class(
      gtk_widget_get_style_context(self->priv->menu_viewport),
      GTK_STYLE_CLASS_SIDEBAR);

  fuk_window_menu_init(self);

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

static void
fuk_window_menu_init(FukWindow * self)
{
  guint n_page_types = 0;
  GType * page_types = g_type_children(FUK_TYPE_PAGE,&n_page_types);

  GtkWidget * page_group = gtk_widget_new(
      FUK_TYPE_PAGE_GROUP,
      "icon-name","folder-symbolic",
      "title","Accounting",
      "description","Accounting options",
      "expanded",FALSE,
      "visible",TRUE,
      NULL);

  gtk_list_box_insert(
      GTK_LIST_BOX(self->priv->menu_box),
      page_group,
      -1);

  GtkWidget * page_group2 = gtk_widget_new(
      FUK_TYPE_PAGE_GROUP,
      "icon-name","folder-symbolic",
      "title","Inventory",
      "description","Inventory options",
      "expanded",FALSE,
      "visible",TRUE,
      NULL);

  gtk_list_box_insert(
      GTK_LIST_BOX(self->priv->menu_box),
      page_group2,
      -1);

  GtkWidget * page_group3 = gtk_widget_new(
        FUK_TYPE_PAGE_GROUP,
        "icon-name","folder-symbolic",
        "title","Billing",
        "description","Billing options",
        "expanded",FALSE,
        "visible",TRUE,
        NULL);

  gtk_list_box_insert(
      GTK_LIST_BOX(self->priv->menu_box),
      page_group3,
      -1);

  GtkWidget * page_group4 = gtk_widget_new(
	 FUK_TYPE_PAGE_GROUP,
	 "icon-name","folder-symbolic",
	 "title","Reporting",
	 "description","Reporting options",
	 "expanded",FALSE,
	 "visible",TRUE,
	 NULL);

     gtk_list_box_insert(
	 GTK_LIST_BOX(self->priv->menu_box),
	 page_group4,
	 -1);

  for(gint page_type_index = 0; page_type_index < n_page_types; page_type_index ++ )
    {
      GType page_type = page_types[page_type_index];
      //const gchar * category = "Otro";
      const gchar * title = g_type_name(page_type);
      const gchar * description = "Open new page";
      const gchar * icon = "tab-new-symbolic";
      /*TODO: configuration reading */

      GtkWidget * page_option =
	  gtk_widget_new(
	      FUK_TYPE_PAGE_OPTION,
	      "icon-name",icon,
	      "title",title,
	      "description",description,
	      "page-type",page_type,
	      "visible",TRUE,
	      NULL);

      fuk_page_group_add_page_option(
	  FUK_PAGE_GROUP(page_group),
	  page_option);

      g_signal_connect_swapped(
	  G_OBJECT(page_option),
	  "clicked",
	  fuk_window_page_option_clicked,
	  self);
    }
}
