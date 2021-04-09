from django.db import models

# Create your models here.

class loc(models.Model):
    index = models.IntegerField()
    lats = models.FloatField()
    lngs= models.FloatField()
