from PIL import Image
import numpy as np
from matplotlib import pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable
import matplotlib.colors as colors

import click


@click.command()
@click.option('--input', help='Input png file')
@click.option('--output', help='output directory.')

def create_colorEdtimage(input, output):
    img_ori = Image.open(input)
    img_new = Image.new("RGB", img_ori.size)
    img_new.paste(img_ori)
    img = np.array(img_new)
    print(img.shape)

    img_new = Image.new("L", img_ori.size)
    img_new = np.array(img_new)
    print(img_new.shape)

    cut_off = 50
    for x in range(img.shape[0]):
        for y in range(img.shape[1]):
            if (img[x][y][0] < cut_off):
                img_new[x][y] = img[x][y][0]
            else:
                img_new[x][y] = cut_off
            img_new[x][y] = img[x][y][0]

    ax = plt.subplot()
    im = ax.imshow(img_new)
    # create an Axes on the right side of ax. The width of cax will be 5%
    # of ax and the padding between cax and ax will be fixed at 0.05 inch.
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.05)

    pcm = ax.pcolor(img_new ,norm=colors.PowerNorm(gamma=0.5),
                    shading='auto', cmap='YlGnBu')
    plt.colorbar(pcm, cax=cax, extend='max')

    # plt.colorbar(im, cax=cax)

    plt.show()

    return img_new


if __name__ == "__main__":
    # img_ori = Image.open('./../../edt_grids_RT147_256/EAC_256/edtplane_42.png')
    # img_ori = Image.open('./../../edt_grids_RT147_256/TMJ_256/edtplane_42.png')
    # img_ori = Image.open('./../../edt_grids_RT147_256/Sinus_+_Dura_256/edtplane_42.png')
    # img_ori = Image.open('./../../resources/edt_grids/sinus_1209_256/Segments1_256/edtplane_42.png')

    create_colorEdtimage()