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

struct _FukPageGroupPrivate
{
  gchar * icon_name;
  gchar * title;
  gchar * description;
  gboolean expanded;
  gboolean item_activated;

  GtkWidget * header;
  GtkWidget * header_icon;
  GtkWidget * header_arrow;
  GtkWidget * header_description;

  GtkWidget * body;
  GtkWidget * revealer;
  GtkWidget * menu;
};

struct _FukPageOptionPrivate
{
  gchar * icon_name;
  gchar * title;
  gchar * description;
  GType	  page_type;

  GtkWidget * header;
  GtkWidget * header_icon;
  GtkWidget * header_description;
};

enum FukPageGroupProp
{
  FUK_PROP_G_ICON_NAME = 1,
  FUK_PROP_G_TITLE,
  FUK_PROP_G_DESCRIPTION,
  FUK_PROP_G_EXPANDED,
  FUK_PROP_G_N,
};

enum FukPageOptionProp
{
  FUK_PROP_O_ICON_NAME = 1,
  FUK_PROP_O_TITLE,
  FUK_PROP_O_DESCRIPTION,
  FUK_PROP_O_PAGE_TYPE,
  FUK_PROP_O_N,
};

enum FukPageGroupSignal
{
  FUK_SIGNAL_G_TOGGLE,
  FUK_SIGNAL_G_N,
};

enum FukPageOptionSignal
{
  FUK_SIGNAL_O_CLICKED,
  FUK_SIGNAL_O_N,
};

static GParamSpec *
fuk_page_group_properties[FUK_PROP_G_N] = {0,};

static GParamSpec *
fuk_page_option_properties[FUK_PROP_O_N] = {0,};

static guint
fuk_page_group_signals[FUK_SIGNAL_G_N] = {0,};

static guint
fuk_page_option_signals[FUK_SIGNAL_O_N] = {0,};

G_DEFINE_TYPE_WITH_PRIVATE(FukPageGroup,fuk_page_group,GTK_TYPE_LIST_BOX_ROW)
G_DEFINE_TYPE_WITH_PRIVATE(FukPageOption,fuk_page_option,GTK_TYPE_LIST_BOX_ROW)

/*
 * FukPageGroup
 *
 * */

static void
fuk_page_group_toggle(FukPageGroup * self)
{
  if(!self->priv->item_activated)
    {
      g_object_set(
            G_OBJECT(self),
            "expanded",
            !self->priv->expanded,
            NULL);
    }
  else
    {
      self->priv->item_activated = FALSE;
    }
}

static void
fuk_page_group_row_activated(FukPageGroup * self,GtkListBoxRow * row)
{
  self->priv->item_activated = TRUE;
  if(FUK_IS_PAGE_OPTION(row))
    {
      g_signal_emit_by_name(row,"clicked",NULL);
    }
}

static void
fuk_page_group_set_property(GObject      *object,
			    guint         property_id,
			    const GValue *value,
			    GParamSpec   *pspec)
{
  FukPageGroup * self = FUK_PAGE_GROUP(object);
  gchar * new_description = NULL;

  switch(property_id)
    {
      case FUK_PROP_G_ICON_NAME:
	g_clear_pointer(&(self->priv->icon_name),g_free);
	self->priv->icon_name = g_value_dup_string(value);
	gtk_image_set_from_icon_name(
	    GTK_IMAGE(self->priv->header_icon),
	    self->priv->icon_name,
	    GTK_ICON_SIZE_MENU);
	break;
      case FUK_PROP_G_TITLE:
	g_clear_pointer(&(self->priv->title),g_free);
	self->priv->title = g_value_dup_string(value);
	new_description = g_strdup_printf(
	    "<b>%s</b>\n<small>%s</small>",
	    self->priv->title,
	    self->priv->description);
	gtk_label_set_markup(
	    GTK_LABEL(self->priv->header_description),
	    new_description);
	g_free(new_description);
      	break;
      case FUK_PROP_G_DESCRIPTION:
	g_clear_pointer(&(self->priv->description),g_free);
	self->priv->description = g_value_dup_string(value);
	new_description = g_strdup_printf(
	    "<b>%s</b>\n<small>%s</small>",
	    self->priv->title,
	    self->priv->description);
	gtk_label_set_markup(
	    GTK_LABEL(self->priv->header_description),
	    new_description);
	g_free(new_description);
      	break;
      case FUK_PROP_G_EXPANDED:
	self->priv->expanded = g_value_get_boolean(value);
	if(self->priv->expanded)
	  {
	    gtk_image_set_from_icon_name(
		GTK_IMAGE(self->priv->header_arrow),
		"pan-down-symbolic",
		GTK_ICON_SIZE_MENU);
	  }
	else
	  {
	    gtk_image_set_from_icon_name(
		GTK_IMAGE(self->priv->header_arrow),
		"pan-end-symbolic",
		GTK_ICON_SIZE_MENU);
	  }

	gtk_revealer_set_reveal_child(
	    GTK_REVEALER(self->priv->revealer),
	    self->priv->expanded);

	break;
      default:
           G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
           break;
    }
}

static void
fuk_page_group_get_property(GObject    *object,
			    guint       property_id,
			    GValue     *value,
			    GParamSpec *pspec)
{
  FukPageGroup * self = FUK_PAGE_GROUP(object);

  switch(property_id)
    {
    case FUK_PROP_G_ICON_NAME:
      g_value_set_string(value,self->priv->icon_name);
      break;
    case FUK_PROP_G_TITLE:
      g_value_set_string(value,self->priv->title);
      break;
    case FUK_PROP_G_DESCRIPTION:
      g_value_set_string(value,self->priv->description);
      break;
    case FUK_PROP_G_EXPANDED:
      g_value_set_boolean(value,self->priv->expanded);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
fuk_page_group_init(FukPageGroup * self)
{
  self->priv = fuk_page_group_get_instance_private(self);

  self->priv->body = gtk_widget_new(
      GTK_TYPE_BOX,
      "orientation",GTK_ORIENTATION_VERTICAL,
      "visible",TRUE,
      NULL);

  self->priv->header = gtk_widget_new(
      GTK_TYPE_BOX,
      "orientation",GTK_ORIENTATION_HORIZONTAL,
      "border-width",5,
      "visible",TRUE,
      NULL);

  self->priv->header_icon = gtk_widget_new(
      GTK_TYPE_IMAGE,
      "icon-name","emblem-system-symbolic",
      "icon-size",GTK_ICON_SIZE_MENU,
      "valign",GTK_ALIGN_START,
      "visible",TRUE,
      NULL);

  self->priv->header_arrow = gtk_widget_new(
      GTK_TYPE_IMAGE,
      "icon-name","pan-end-symbolic",
      "icon-size",GTK_ICON_SIZE_MENU,
      "valign",GTK_ALIGN_START,
      "visible",TRUE,
      NULL);

  self->priv->header_description = gtk_widget_new(
      GTK_TYPE_LABEL,
      "label","<b></b>\n<small></small>",
      "xalign",0.0,
      "use-markup", TRUE,
      "visible", TRUE,
      NULL);

  self->priv->revealer = gtk_widget_new(
      GTK_TYPE_REVEALER,
      "transition-type",GTK_REVEALER_TRANSITION_TYPE_SLIDE_DOWN,
      "transition-duration",200,
      "visible",TRUE,
      NULL);

  self->priv->menu = gtk_widget_new(
      GTK_TYPE_LIST_BOX,
      "activate-on-single-click",TRUE,
      "visible",TRUE,
      NULL);


  gtk_box_pack_start(
      GTK_BOX(self->priv->header),
      self->priv->header_icon,
      FALSE,
      FALSE,
      5);

  gtk_box_pack_start(
      GTK_BOX(self->priv->header),
      self->priv->header_description,
      TRUE,
      TRUE,
      5);

  gtk_box_pack_end(
      GTK_BOX(self->priv->header),
      self->priv->header_arrow,
      FALSE,
      FALSE,
      0);

  gtk_box_pack_start(
      GTK_BOX(self->priv->body),
      self->priv->header,
      FALSE,
      FALSE,
      0);

  gtk_box_pack_end(
     GTK_BOX(self->priv->body),
     self->priv->revealer,
     FALSE,
     FALSE,
     0);


  gtk_container_add(
      GTK_CONTAINER(self),
      self->priv->body);

  gtk_container_add(
      GTK_CONTAINER(self->priv->revealer),
      self->priv->menu);

  gtk_list_box_row_set_selectable(
      GTK_LIST_BOX_ROW(self),
      FALSE);


  g_signal_connect(
      self,
      "toggle",
      fuk_page_group_toggle,
      NULL);

  g_signal_connect_swapped(
      G_OBJECT(self->priv->menu),
      "row-activated",
      G_CALLBACK(fuk_page_group_row_activated)
      ,self);

  /*gtk_style_context_add_class(
      gtk_widget_get_style_context(self->priv->menu),
      GTK_STYLE_CLASS_BACKGROUND);*/
}

static void
fuk_page_group_class_init(FukPageGroupClass * klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = fuk_page_group_set_property;
  object_class->get_property = fuk_page_group_get_property;


  fuk_page_group_properties[FUK_PROP_G_ICON_NAME] =
  g_param_spec_string ("icon-name",
		       "Icon name",
		       "The icon name in the theme.",
		       NULL  /* default value */,
		       G_PARAM_READWRITE);

  fuk_page_group_properties[FUK_PROP_G_TITLE] =
  g_param_spec_string ("title",
		     "Title",
		     "Title of page group",
		     NULL  /* default value */,
		     G_PARAM_READWRITE);

  fuk_page_group_properties[FUK_PROP_G_DESCRIPTION] =
  g_param_spec_string ("description",
		   "Description",
		   "Description of page group",
		   NULL  /* default value */,
		   G_PARAM_READWRITE);

  fuk_page_group_properties[FUK_PROP_G_EXPANDED] =
  g_param_spec_boolean("expanded",
		 "Expanded",
		 "Indicate if the is expanded",
		 FALSE  /* default value */,
		 G_PARAM_READWRITE);

  fuk_page_group_signals[FUK_SIGNAL_G_TOGGLE] =
  g_signal_newv ("toggle",
	     G_TYPE_FROM_CLASS (klass),
	      G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
	      NULL,
	      NULL,
	      NULL,
	      NULL,
	      G_TYPE_NONE,
	      0,
	      NULL);

  g_object_class_install_properties (object_class,
				     FUK_PROP_G_N,
				     fuk_page_group_properties);
}

void
fuk_page_group_add_page_option(FukPageGroup * page_group,
			       GtkWidget * page_option)
{
  gtk_list_box_insert(
    GTK_LIST_BOX(page_group->priv->menu),
    page_option,
    -1);
}

void
fuk_page_group_remove_page_option(FukPageGroup * page_group,
				  GtkWidget * page_option)
{
  gtk_container_remove(
    GTK_CONTAINER(page_group->priv->menu),
    page_option);
}


/*
 * FukPageOption
 *
 * */

static void
fuk_page_option_set_property(GObject      *object,
			    guint         property_id,
			    const GValue *value,
			    GParamSpec   *pspec)
{
  FukPageOption * self = FUK_PAGE_OPTION(object);
  gchar * new_description = NULL;

  switch(property_id)
    {
      case FUK_PROP_O_ICON_NAME:
	g_clear_pointer(&(self->priv->icon_name),g_free);
	self->priv->icon_name = g_value_dup_string(value);
	gtk_image_set_from_icon_name(
	    GTK_IMAGE(self->priv->header_icon),
	    self->priv->icon_name,
	    GTK_ICON_SIZE_MENU);
	break;
      case FUK_PROP_O_TITLE:
	g_clear_pointer(&(self->priv->title),g_free);
	self->priv->title = g_value_dup_string(value);
	new_description = g_strdup_printf(
	  "<b>%s</b>\n<small>%s</small>",
	  self->priv->title,
	  self->priv->description);
	gtk_label_set_markup(
	    GTK_LABEL(self->priv->header_description),
	    new_description);
	g_free(new_description);
	break;
      case FUK_PROP_O_DESCRIPTION:
	g_clear_pointer(&(self->priv->description),g_free);
	self->priv->description = g_value_dup_string(value);
	new_description = g_strdup_printf(
	  "<b>%s</b>\n<small>%s</small>",
	  self->priv->title,
	  self->priv->description);
	gtk_label_set_markup(
	    GTK_LABEL(self->priv->header_description),
	    new_description);
	g_free(new_description);
	break;
      case FUK_PROP_O_PAGE_TYPE:
	self->priv->page_type = g_value_get_gtype(value);
	break;
      default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	break;
    }
}

static void
fuk_page_option_get_property(GObject    *object,
			    guint       property_id,
			    GValue     *value,
			    GParamSpec *pspec)
{
  FukPageOption * self = FUK_PAGE_OPTION(object);

  switch(property_id)
    {
    case FUK_PROP_O_ICON_NAME:
      g_value_set_string(value,self->priv->icon_name);
      break;
    case FUK_PROP_O_TITLE:
      g_value_set_string(value,self->priv->title);
      break;
    case FUK_PROP_O_DESCRIPTION:
      g_value_set_string(value,self->priv->description);
      break;
    case FUK_PROP_O_PAGE_TYPE:
      g_value_set_gtype(value,self->priv->page_type);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
fuk_page_option_init(FukPageOption * self)
{
  self->priv = fuk_page_option_get_instance_private(self);

  self->priv->header = gtk_widget_new(
        GTK_TYPE_BOX,
        "orientation",GTK_ORIENTATION_HORIZONTAL,
        "border-width",5,
        "visible",TRUE,
        NULL);

  self->priv->header_icon = gtk_widget_new(
      GTK_TYPE_IMAGE,
      "icon-name","emblem-system-symbolic",
      "icon-size",GTK_ICON_SIZE_MENU,
      "valign",GTK_ALIGN_START,
      "visible",TRUE,
      NULL);

  self->priv->header_description = gtk_widget_new(
      GTK_TYPE_LABEL,
      "label","<b></b>\n<small></small>",
      "xalign",0.0,
      "use-markup", TRUE,
      "visible", TRUE,
      NULL);


  gtk_box_pack_start(
      GTK_BOX(self->priv->header),
      self->priv->header_icon,
      FALSE,
      FALSE,
      5);

  gtk_box_pack_start(
      GTK_BOX(self->priv->header),
      self->priv->header_description,
      TRUE,
      TRUE,
      5);

  gtk_container_add(
      GTK_CONTAINER(self),
      self->priv->header);

  gtk_list_box_row_set_selectable(
    GTK_LIST_BOX_ROW(self),
    FALSE);
}

static void
fuk_page_option_class_init(FukPageOptionClass * klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = fuk_page_option_set_property;
  object_class->get_property = fuk_page_option_get_property;

  fuk_page_option_properties[FUK_PROP_O_ICON_NAME] =
  g_param_spec_string ("icon-name",
		       "Icon name",
		       "The icon name in the theme.",
		       NULL  /* default value */,
		       G_PARAM_READWRITE);

  fuk_page_option_properties[FUK_PROP_O_TITLE] =
  g_param_spec_string ("title",
		     "Title",
		     "Title of page group",
		     NULL  /* default value */,
		     G_PARAM_READWRITE);

  fuk_page_option_properties[FUK_PROP_O_DESCRIPTION] =
  g_param_spec_string ("description",
		   "Description",
		   "Description of page group",
		   NULL  /* default value */,
		   G_PARAM_READWRITE);

  fuk_page_option_properties[FUK_PROP_O_PAGE_TYPE] =
  g_param_spec_gtype("page-type",
		 "Page type",
		 "Type of page to be instanced",
		 FUK_TYPE_PAGE  /* default value */,
		 G_PARAM_READWRITE);

  fuk_page_option_signals[FUK_SIGNAL_O_CLICKED] =
    g_signal_newv ("clicked",
	     G_TYPE_FROM_CLASS (klass),
  	      G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
  	      NULL /* closure */,
  	      NULL /* accumulator */,
  	      NULL /* accumulator data */,
  	      NULL /* C marshaller */,
  	      G_TYPE_NONE /* return_type */,
  	      0     /* n_params */,
  	      NULL  /* param_types */);

  g_object_class_install_properties (object_class,
				     FUK_PROP_O_N,
				     fuk_page_option_properties);
}

