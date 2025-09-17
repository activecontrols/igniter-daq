#include "imgui.h"
#include "implot.h"
#include "math.h"

#include <iostream>

void render_loop() {
  static int frame_counter = 0;
  frame_counter++;

  ImGuiIO io = ImGui::GetIO();

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(io.DisplaySize); // io = ImGui::GetIO()
  ImGui::Begin("MainWindow", nullptr,
               ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus);
  // Get available width for plots
  ImVec2 avail = ImGui::GetContentRegionAvail();

  // Sine Plot
  static float xs[1000], ys[1000];
  for (int i = 0; i < 1000; ++i) {
    xs[i] = i * 0.01f;
    ys[i] = sin(xs[i] + frame_counter / 100.0);
  }
  if (ImPlot::BeginPlot("Sine Wave")) { // half height
    ImPlot::PlotLine("sin(x)", xs, ys, 1000);
    ImPlot::EndPlot();
  }

  // Cosine Plot
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < 1000; ++i) {
      ys[i] = cos(xs[i] + (frame_counter + j * 20) / 10.0);
    }
    ImGui::PushID(j);
    if (ImPlot::BeginPlot("Cosine Wave", ImVec2(-1, 100))) { // half height
      ImPlot::PlotLine("cos(x)", xs, ys, 1000);
      ImPlot::EndPlot();
    }
    ImGui::PopID();
  }

  ImGui::Text("%f", io.Framerate);
  ImGui::End();
}
