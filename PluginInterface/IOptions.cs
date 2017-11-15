using System;
using System.Collections.Generic;
using System.Text;

namespace PluginInterface
{
    public interface IOptions
    {
        //Permet au plugin .net de gerer des Options de la manière dont il le souhaite via le bouton "Options"
        //Que ce soit des fichier de config, des formulaire ect ....
        void ShowDialog();
    }
}
