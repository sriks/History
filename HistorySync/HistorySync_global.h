#ifndef HISTORYSYNC_GLOBAL_H
#define HISTORYSYNC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HISTORYSYNC_LIBRARY)
#  define HISTORYSYNCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HISTORYSYNCSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HISTORYSYNC_GLOBAL_H
