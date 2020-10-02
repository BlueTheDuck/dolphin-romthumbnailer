#!/bin/env sh
cd build || exit -1;
cmake -DKDE_INSTALL_USE_QT_SYS_PATHS=ON -DCMAKE_INSTALL_PREFIX=`kf5-config --prefix` ..;
make || exit;
sudo make install || exit;
kbuildsycoca5