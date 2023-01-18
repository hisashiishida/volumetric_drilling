//==============================================================================
/*
    Software License Agreement (BSD License)
    Copyright (c) 2019-2022, AMBF
    (https://github.com/WPI-AIM/ambf)

    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following
    disclaimer in the documentation and/or other materials provided
    with the distribution.

    * Neither the name of authors nor the names of its contributors may
    be used to endorse or promote products derived from this software
    without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    \author    <hishida3@jhu.edu>
    \author    Hisashi Ishida
*/
//==============================================================================

#ifndef EDT_MANAGER_H
#define EDT_MANAGER_H

// #include "edt_utils_spine.h"
#include <iostream>
#include <vector>
#include "EdtReader/EdtReader.h"

// To display color
#include <algorithm>
#include <sstream>
#include <iterator>

#include <afFramework.h>
#include "camera_panel_manager.h"

using namespace std;

class EdtContainer
{
public:
    float m_dist_object; //Do we need this???
    string m_name;
    vector<int> m_rgb;

    inline float getForceThreshold() { return m_forceThreshold; }
    inline float getAudioThreshold() { return m_forceThreshold; }

    bool setThresholds(float force_thres, float audio_thres)
    {
        if (force_thres > 0. && audio_thres > 0.)
        {
            m_forceThreshold = force_thres;
            m_audioThreshold = audio_thres;
            return true;
        }
        return false;
    }

    void load_grid(string edt_path)
    {
        float *values_buffer;
        unsigned int res[3];
        edt_reader(edt_path, &values_buffer, res);
        m_edt_grid = new Array3d<float>(values_buffer, res);
    }
    void get_resolution(unsigned int *resolution)
    {
        *resolution = (m_edt_grid)->res[0];
        *(resolution + 1) = (m_edt_grid)->res[1];
        *(resolution + 2) = (m_edt_grid)->res[2];
    }
    void print_info()
    {
        // RGB vect to string
        std::ostringstream vts;
        std::copy(m_rgb.begin(), m_rgb.end(),
                  std::ostream_iterator<int>(vts, ", "));

        printf("name: %s || color: %s \n", this->m_name.c_str(), vts.str().c_str());
    }

protected: //Why these are protected?
    Array3d<float> *m_edt_grid;
    float m_forceThreshold;
    float m_audioThreshold;
};

class EdtManager
{
public:
    //Constructor
    EdtManager();

    //Initialize the Edt Manager
    void initialize(afWorldPtr a_worldPtr, CameraPanelManager *a_panelManager, p_opt::variables_map &var_map);

    void intializeShader();

    void load_edt(string file_path);

    void update(double dt);

    void initializeLabels();

    std::vector<float> getDistances(cTransform m_T_d);
    float getCloestDistance(cTransform m_T_d);

    void activateEdtAssistance(bool force, bool audio, bool visual)
    {
        m_force = force;
        m_audio = audio;
        m_viual = visual;
        cerr << "EDT assistance staus:: \n"
             << "Visual: " << m_visual << ", Force: " << m_force << ", Audio: " << m_audio << endl;
    }

    void print_info()
    {
        printf("------------------------\n");
        printf("Printing edt information\n");
        printf("------------------------\n");
        for (int i = 0; i < m_edt_num; i++)
        {
            m_edts[i]->print_info();
        }
    }

    cVector3d getForceFromEDT(cTransform m_T_d);
    void setForceParam(float const)
    {
        m_force_const = const;
    }

    bool m_enableForce, m_enableAudio, m_enableVisual;
    int m_edt_num;
    string edt_root_path;

    std::vector<EdtContainer *> m_edts;
    EdtContainer *m_activeEdt = nullptr;

    //Audio related
    cAudioSource *m_audioSource = nullptr;
    cAudioBuffer *m_audioBuffer = nullptr;
    cAudioDevice *m_audioDevice = nullptr;
    AudioState m_audioState;
    void setAudioPitch(double pitch);

    //Visual related
    afCameraPtr m_mainCamera;
    CameraPanelManager *m_panelManager;

    //Force related
    float m_force_const = 1.0;
};

#endif
