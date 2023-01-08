from django.shortcuts import render, redirect
from django.contrib.auth.decorators import login_required
import random

from apps.emosy.models import Monitor

@login_required
def home(request):
    data = {
        'monitors': Monitor.objects.all(),
        }
    return render(
            request,
            "emosy/main.html",
            context = data
            )

@login_required
def create_monitor(request):
    if not (request.method == "POST"):
        return redirect('emosy:home')

    verif_mon_id = False
    while (not verif_mon_id):
        mon_id = random.randint(1000, 9999)
        data = Monitor.objects.filter(uid = mon_id)
        if (len(data) == 0):
            verif_mon_id = True

    new_data = Monitor(
        n_aires = 24,
        name = request.POST['name'],
        uid = mon_id,
        description = request.POST['description'],
        )
    new_data.save()
    return redirect('emosy:home')

@login_required
def edit_monitor(request, mon_id):
    monitor_ = Monitor.objects.get(
        uid = mon_id
        )
    idxs = []
    for i in range(1,25):
        new_val = {
                'idx': i,
                'val': eval(F'monitor_.idx{i}')
                }
        idxs.append(new_val)
    data = {'monitor': monitor_,
            'idxs': idxs}
    return render(
            request,
            "emosy/edit_monitor.html",
            context = data
            )
@login_required
def save_monitor(request):
    if not(request.method == "POST"):
        return redirect('emosy:home')
    monitor_ = Monitor.objects.filter(uid=request.POST['uid'])
    monitor_.update(name=request.POST['name'])
    for i in range(1,25):
        val = request.POST[F'idx{i}']
        print(F'monitor_.update(idx{i}={val}')
        exec(F'monitor_.update(idx{i}={val})')
    return redirect('emosy:home')

