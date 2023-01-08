from django.contrib import admin
from apps.emosy.models import Monitor, ElectricalData, Request

admin.site.register(Monitor)
admin.site.register(ElectricalData)
admin.site.register(Request)
