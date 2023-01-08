from django.conf import settings
from django.conf.urls.static import static
from django.urls import include, re_path, path
from django.views.generic import TemplateView

from apps.emosy import views


app_name = 'emosy'

urlpatterns = [
    re_path(r'^$', views.home, name='home'),
    re_path(
        r'^create_monitor/',
        views.create_monitor,
        name='create_monitor'
        ),
    path('edit_monitor/<mon_id>',
            views.edit_monitor,
            name='edit_monitor'
            ),
    re_path(r'^save_monitor/',
            views.save_monitor,
            name='save_monitor'
            ),
    ]
