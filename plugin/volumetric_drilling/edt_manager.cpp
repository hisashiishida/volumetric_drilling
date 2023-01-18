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

#include "edt_manager.h"

using namespace std;

void EdtManager::EdtManager()
{
    //What should I put in the constructor???
}
void EdtManager::initialize(afWorldPtr a_worldPtr, CameraPanelManager *a_panelManager, p_opt::variables_map &var_map)
{
    // Load edt/sdf related variables
    edt_root = var_map["edt"].as<string>();
    edt_root = edt_root + "/";

    cond = var_map["condition"].as<int>();
    double unit = var_map["unit"].as<double>();
    space_res = var_map["spacial_resolution"].as<double>();
    cerr << "Active condition: " << cond << endl;

    //Activate the modes
    if (cond == 0)
    {
        m_visual = true;
        m_force = true;
        m_audio = true;
    }

    // Setup camera/panel relateed pointer
    m_mainCamera = a_worldPtr->getCamera("main_camera");
    m_panelManager = a_panelManager;

    load_edts(edt_root);
}

void EdtManager::intializeShader(string current_filepath, afVolumePtr a_volumeObject, p_opt::variables_map &var_map)
{
    //Get SDF shader related path
    string sdf_path = var_map["sdf"].as<string>();
    string bone_path = var_map["bone"].as<string>();

    //Define SDF related textures
    cTexture3dPtr sdfTex = cTexture3d::create();
    cMultiImagePtr sdfImages = cMultiImage::create();

    cTexture3dPtr sdfTex1 = cTexture3d::create();
    cMultiImagePtr sdfImages1 = cMultiImage::create();

    string sdfPath = current_filepath + "/" + edt_root + sdf_path + "/edtplane_";
    string sdfPath1 = current_filepath + "/" + edt_root + bone_sdf_path + "_256" + "/edtplane_";

    cerr << "SDF path: " << sdfPath << endl;
    cerr << "SDF path1: " << sdfPath1 << endl;

    int num_sdfimages = sdfImages->loadFromFiles(sdfPath, "png", 512);
    int num_sdfimages1 = sdfImages1->loadFromFiles(sdfPath1, "png", 512);

    cerr << "# of sdfimages in SDF path: " << num_sdfimages << endl;
    cerr << "# of sdfimages in SDF path1: " << num_sdfimages1 << endl;

    if (num_sdfimages > 0)
    {
        sdfTex->setImage(sdfImages);
        sdfTex1->setImage(sdfImages1);
        a_volumeObject->getInternalVolume()->m_metallicTexture = sdfTex;
        a_volumeObject->getInternalVolume()->m_metallicTexture->setTextureUnit(GL_TEXTURE3);
        a_volumeObject->getInternalVolume()->m_roughnessTexture = sdfTex1;
        a_volumeObject->getInternalVolume()->m_roughnessTexture->setTextureUnit(GL_TEXTURE4);

        cerr << "FOUND SDF TEXTURE" << endl;
    }
    else
    {
        cerr << "FAILED TO FOUND SDF TEXTURE" << endl;
    }
}

//TODO: Change this to load data for the yaml file.
void EdtManager::load_edts(string file_path)
{
    //Get this number from the yaml file
    m_edt_num = 15;

    for (int i = 0; i < m_edt_num; i++)
    {
        //TODO: Change this place to import info from yaml
        string name;
        vector<int> rgb = vector<int>{122, 100, 10};
        vector<float> thres = vector<double>{1.0, 2.0};

        EdtContainer *edt = new EdtContainer();
        edt->m_name = name;
        edt->m_rgb = rgb;
        edt->setThresholds(thres[0], thres[1]);
        m_edts.push_back(edt)
    }
}

void EdtManager::update(double dt)
{
    // Not yet implemented
}

void EdtManager::initializeLabels();

std::vector<float> EdtManager::getDistances(cTransform m_T_d);
float EdtManager::getCloestDistance(cTransform m_T_d);
