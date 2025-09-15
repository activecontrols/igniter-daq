import TKinterModernThemes as TKMT
from matplotlib.axes._axes import Axes
from matplotlib.figure import Figure
import time
import numpy as np
import random


DO_COUNT = 5


sample_autoseq = [
    {"Time": 0,   "DO": "DO_1", "State": "On"},
    {"Time": 0.5, "DO": "DO_2", "State": "On"},
    {"Time": 2,   "DO": "DO_1", "State": "Off"}
]

class TKGraph:
    def __init__(self, canvas, fig: Figure, ax: Axes, backgroundcolor: str, accentcolor: str, labels):
        self.canvas = canvas
        self.fig = fig
        self.ax = ax
        self.backgroundcolor = backgroundcolor
        self.accentcolor = accentcolor
        self.labels = labels

        self.xs = []
        self.points_1 = []
        self.points_2 = []
        self.last_x = 0
        self.last_y_1 = 50
        self.last_y_2 = 30

        self.scatter1 = ax.scatter([], [], c=self.accentcolor)
        self.scatter2 = ax.scatter([], [], c='blue')
        ax.axhline(y=70, color='r', linestyle='--', label='redline')
        self.ax.set_xlabel("Time")
        self.ax.set_ylim(0, 100)
        self.fig.legend(self.labels)


    def update(self):
        self.xs.append(self.last_x)
        self.points_1.append(self.last_y_1)
        self.points_2.append(self.last_y_2)
        self.last_x += 1
        self.last_y_1 += (random.random() * 5) - 2.4
        self.last_y_2 += (random.random() * 5) - 2.4

        self.xs = self.xs[-20:]
        self.points_1 = self.points_1[-20:]
        self.points_2 = self.points_2[-20:]

        self.scatter1.set_offsets(np.c_[self.xs, self.points_1])
        self.scatter2.set_offsets(np.c_[self.xs, self.points_2])
        self.ax.set_xlim(self.last_x - 20, self.last_x)

        self.canvas.draw()


class App(TKMT.ThemedTKinterFrame):
    def __init__(self):
        super().__init__("Igniter DAQ")
        self.root.tk.call("source", 'cust_ui.tcl')
        #self.root.attributes("-fullscreen", True)

        self.control_frame = self.addLabelFrame("Control")
        self.control_frame.Button("START", None, style=TKMT.ThemeStyles.ButtonStyles.AccentButton)
        self.control_frame.Button("ABORT", None, style='Red_' + TKMT.ThemeStyles.ButtonStyles.AccentButton, col=1)
        self.control_frame.Treeview(['Time', 'DO', 'State'], [50, 50, 50], 10, sample_autoseq, '', colspan=2)

        self.do_frame = self.control_frame.addFrame("do frame", colspan=2, padx=(0,0), pady=(0,0))
        for do_num in range(DO_COUNT):
            self.do_frame.ToggleButton(f"DO_{do_num}", None, col=do_num)

        self.serial_frame = self.control_frame.addLabelFrame("Serial Command Interface", colspan=2, padx=(5,5), pady=(5,5))
        self.serial_frame.Entry(None, widgetkwargs={'width': 50})
        self.serial_frame.AccentButton("Send", None, col=1)
        
        self.nextCol()
        self.data_frame = self.addLabelFrame("Data")
        self.graph_1 = TKGraph(*self.data_frame.matplotlibFrame("Graph 1", figsize=(4,2.5), toolbar=False, padx=0, pady=0), ['PT0', 'PT1'])
        self.graph_2 = TKGraph(*self.data_frame.matplotlibFrame("Graph 2", figsize=(4,2.5), toolbar=False, padx=0, pady=0), ['PT2', 'PT3'])
        self.graph_3 = TKGraph(*self.data_frame.matplotlibFrame("Graph 3", figsize=(4,2.5), toolbar=False, padx=0, pady=0), ['TC_LOX', 'TC_IPA'])
        
        self.root.after(500, self.periodic)
        #self.root.after(2000, self.fullscreen)

        self.debugPrint()
        self.run(onlyFrames=False)

    def periodic(self):
        s = time.time()
        self.graph_1.update()
        self.graph_2.update()
        self.graph_3.update()
        print('e', time.time() - s)
        self.root.after(100, self.periodic) #Call every 0.1 seconds to keep UI updated

    def fullscreen(self):
        self.root.attributes("-fullscreen", True)



if __name__ == '__main__':
    app = App()