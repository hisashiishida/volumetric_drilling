import os
from pathlib import Path
import click

@click.command()
@click.option('--input', help='Input in resources/volumes ex) spine1_512')
@click.option('--output', help='output directory. ex) edt_grids_256_spine1')

def create_edt_spine(input, output):

    edtexec_p = Path("./../EDT/cmake-build/bin/EDTFromGrid")
    imglist_p = Path("./resources/volumes/" + input + "/" + input  + ".txt")

    print("Creating image using:", imglist_p)


    for name, value in spine_dict.items():
        print(f"generate edt for {name}. ({value})")

        dst_p = Path(f"./" + output + "/" + name + ".edt")
        # Execute command to generate EDT.
        cmd = f"{edtexec_p} --in {imglist_p} --id {value} --out {dst_p}"
        print(f"executing: {cmd}")
        os.system(cmd)


spine_dict = {
    "Vertebral_foramen": "181 228 255",
    "L1_minus_drilling":  "219 244 20",
    "L2_minus_drilling":  "182 156 219",
    "L3_minus_drilling":  "214 230 130",
}

if __name__ == "__main__":
    create_edt_spine()