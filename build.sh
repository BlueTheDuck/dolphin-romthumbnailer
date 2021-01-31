#!/bin/env sh
[ -d build ] || mkdir build
cd build \
  && cmake -DKDE_INSTALL_USE_QT_SYS_PATHS=ON -DCMAKE_INSTALL_PREFIX=`kf5-config --prefix` .. \
  && make \
  && cd tests && ctest && cd .. \
  && [ "$1" != "dry" ] \
  && sudo make uninstall \
  && sudo make install \
  && kbuildsycoca5