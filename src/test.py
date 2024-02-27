scene_template = 'case {number}: return Scene({color}, {color}, {duration});'

for i in range(1, 100):
    print(scene_template.format(
        number=i,
        color=f'S{int(i // (100 / 8)) + 1}',
        duration=(100 + i),
        ))
