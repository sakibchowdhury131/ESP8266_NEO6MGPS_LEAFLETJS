from django.shortcuts import render
from .models import loc

# Create your views here.

def home(request):
    
    positions = loc.objects.all()
    poss = []

    for x in positions:
        poss.append([x.lats, x.lngs])

    return render (request, "home.html", {'posss': poss})