/* This file is part of Clementine.
   Copyright 2010, David Sansome <me@davidsansome.com>

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COVERPROVIDERS_H
#define COVERPROVIDERS_H

#include <QMutex>
#include <QObject>

class AlbumCoverFetcherSearch;
class CoverProvider;

// This is a singleton, a global repository for cover providers.
// Each one of those has to register with CoverProviders' instance by invoking
// "CoverProviders::instance().AddCoverProvider(this)". Providers are
// automatically unregistered from the repository when they are deleted.
// The class is thread safe except for the initialization.
class CoverProviders : public QObject {
  Q_OBJECT

public:
  // This performs lazy initialization of the CoverProviders which is not thread-safe!
  static CoverProviders& instance() {
    static CoverProviders instance_;
    return instance_;
  }

  // Lets a cover provider to register itself in the repository.
  void AddProvider(CoverProvider* provider);
  void RemoveProvider(CoverProvider* provider);

  // Returns a list of cover providers
  QList<CoverProvider*> List() const { return cover_providers_; }
  // Returns true if this repository has at least one registered provider.
  bool HasAnyProviders() const { return !cover_providers_.isEmpty(); }

  int NextId();

private slots:
  void ProviderDestroyed();

private:
  CoverProviders();
  ~CoverProviders() {}
  Q_DISABLE_COPY(CoverProviders);

  QList<CoverProvider*> cover_providers_;
  QMutex mutex_;

  QAtomicInt next_id_;
};

#endif // COVERPROVIDERS_H
