#include "pal_tf_lookup/TfStream.h"

#include "pal_tf_lookup/Subscription.h"
#include <tf/tfMessage.h>

namespace pal
{
  TfStream::TfStream(ros::NodeHandle& nh, const std::string& id,
      const LookupFun& lookup_fun) :
    _id(id),
    _pub(nh.advertise<tf::tfMessage>(_id, 10)),
    _lookup_fun(lookup_fun)
  {}

  TfStream::~TfStream()
  {}

  void TfStream::updateTransforms(const TrVect& transforms)
  {
    _transforms = transforms;
  }

  void TfStream::publish()
  {
    tf::tfMessage m;
    for (auto& t : _transforms)
    {
      geometry_msgs::TransformStamped trans;
      if (_lookup_fun(t.target, t.source, ros::Time(0), trans))
        m.transforms.push_back(trans);
    }

    _pub.publish(m);
  }
}
