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

#include "fukurou.h"

static void fukurou_app_activate(GtkApplication * app);

int main(int argc,char ** argv)
{
  GtkApplication * fukurou_app = gtk_application_new(
      "org.gnome.desktop.fukurou",
      G_APPLICATION_FLAGS_NONE);

  g_signal_connect(G_OBJECT(fukurou_app),"activate",G_CALLBACK(fukurou_app_activate),NULL);

  return g_application_run(G_APPLICATION(fukurou_app),argc,argv);
}

static void
fukurou_app_activate(GtkApplication * app)
{
  FUK_TYPE_ACCOUNT_PAGE;

  GtkWidget * main_window = fuk_window_new();
  gtk_widget_show(main_window);
  gtk_application_add_window(app,GTK_WINDOW(main_window));
}
