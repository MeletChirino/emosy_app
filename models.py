'''Models for Emosy App'''
from django.db import models
from django.core.validators import (
        MinValueValidator,
        MaxValueValidator,
        )
#python modules
import datetime

class Monitor(models.Model):
    n_aires = models.IntegerField(blank=True)
    name = models.CharField(max_length=10)
    uid = models.PositiveIntegerField(
            validators = [
                MinValueValidator(1000),
                MaxValueValidator(9999),
                ]
            )
    description = models.CharField(max_length=50, blank=True)

    idx1 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx2 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx3 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx4 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx5 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx6 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx7 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx8 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx9 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx10 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx11 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx12 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx13 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx14 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx15 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx16 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx17 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx18 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx19 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx20 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx21 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx22 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx23 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    idx24 = models.PositiveIntegerField(
            validators = [
                MinValueValidator(100),
                MaxValueValidator(999),
                ],
            default = 100
            )
    enabled = models.BooleanField(default=True)
    modified = models.DateTimeField(auto_now=True)
    created = models.DateTimeField(auto_now_add=True)
    def __str__(self):
        return F'{self.name}'

class ElectricalData(models.Model):
    monitor = models.ForeignKey(
            Monitor,
            on_delete = models.CASCADE
            )
    data = models.PositiveIntegerField()
    time = models.PositiveIntegerField() #Ryem time
    modified = models.DateTimeField(auto_now=True)
    created = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return F'Mon::{self.monitor} at {self.time}'

class Request(models.Model):
    monitor = models.ForeignKey(
            Monitor,
            on_delete = models.CASCADE,
            blank = True
            )
    data = models.PositiveIntegerField()
    types = [
            ('SERVER_CONNECTION', 'SERVER_CONNECTION'),
            ('CLOCK_SYNC', 'CLOCK_SYNC'),
            ('DATA_TRANSMISSION', 'DATA_TRANSMISSION'),
            ('OTHER', 'OTHER')
            ]
    type = models.CharField(
            max_length = 20,
            choices = types,
            default = 'DATA_SEND',
            )
    modified = models.DateTimeField(auto_now=True)
    created = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return F"{self.monitor.name} [{self.created}]::{self.type}::{hex(self.data)}"
